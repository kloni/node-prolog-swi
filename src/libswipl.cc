#include <node.h>
#include <SWI-Prolog.h>
#include "libswipl.h"
#include "v8-util.h"

Persistent<Function> Query::constructor;

Handle<Value> CreateException(Isolate* isolate, const char *msg) {
  Handle<Object> result = Object::New(isolate);
  result->Set(String::NewFromUtf8(isolate, "exc"), String::NewFromUtf8(isolate, msg));
  return result;
}

const char* GetExceptionString(term_t term) {
  char *msg;
  term_t msgterms = PL_new_term_refs(2);
  PL_put_term(msgterms, term);
  int rval = PL_call_predicate(NULL, PL_Q_NODEBUG,
      PL_predicate("message_to_string", 2, NULL), msgterms);
  if (rval) {
    rval = PL_get_chars(msgterms + 1, &msg, CVT_ALL);
    return msg;
  } else {
    return "unknown error";
  }
}

void TermType(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  int rval = 0;
  term_t term = PL_new_term_ref();
  rval = PL_chars_to_term(*String::Utf8Value(args[0]), term);
  if (rval) {
    rval = PL_term_type(term);
  }

  args.GetReturnValue().Set(Number::New(isolate, rval));
}

void ExportTerm(const FunctionCallbackInfo<Value>& args, term_t t, Local<Object> result, 
	map<int, std::string>& varnames) {
  Isolate* isolate = args.GetIsolate();

  int rval = 0;
  Handle<Value> val;
  char *c;
  int i = 0;
  double d = 0.0;
  int type = PL_term_type(t);

  if (varnames[t].length()) {
  	
    switch (type) {
    case PL_FLOAT:
      rval = PL_get_float(t, &d);
      val = Number::New(isolate, d);
      break;
    case PL_INTEGER:
      rval = PL_get_integer(t, &i);
      val = Integer::New(isolate, i);
      break;
    default:
      rval = PL_get_chars(t, &c, CVT_ALL);
      val = String::NewFromUtf8(isolate, c);
      break;
    }
    if (rval) {
      result->Set(String::NewFromUtf8(isolate, varnames[t].c_str()), val);
    }
  }
}

Handle<Object> ExportSolution(const FunctionCallbackInfo<Value>& args, term_t t, int len, Local<Object> result_terms,
    map<int, std::string>& varnames) {
  for (int j = 0; j < len; j++) {
    ExportTerm(args, t + j, result_terms, varnames);
  }
  return result_terms;
}

void Initialise(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  int rval;
  const char *plav[3];

  // make the argument vector for Prolog 
  plav[0] = "node";
  plav[1] = "--quiet";
  plav[2] = NULL;

  // initialise Prolog 
  rval = PL_initialise(2, (char **) plav);

  args.GetReturnValue().Set(Number::New(isolate, rval));
}

void Cleanup(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // close Prolog 
  int rval = PL_cleanup(0);

  args.GetReturnValue().Set(Number::New(isolate, rval));
}

module_t GetModule(const FunctionCallbackInfo<Value>& args, int idx) {
  module_t mo = NULL;
  if (args.Length() > idx && !(args[idx]->IsUndefined() || args[idx]->IsNull())
      && args[idx]->IsString()) {
    mo = PL_new_module(PL_new_atom(*String::Utf8Value(args[idx])));
  }
  return mo;
}

Query::Query() {
}
;
Query::~Query() {
}
;

void Query::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, Open);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Query"));
  tpl->InstanceTemplate()->SetInternalFieldCount(3);
  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "next_solution", NextSolution);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "exception", Exception);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Query"),
               tpl->GetFunction());
}

void Query::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    Query* obj = new Query();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void Query::Open(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Query* obj = new Query();
  obj->cb_log = NULL; //&printf;

  module_t module = GetModule(args, 2);
  const char *module_name =
      module ? PL_atom_chars(PL_module_name(module)) : NULL;

  if (obj->cb_log)
      obj->cb_log("Query::Open module: %s\n", module_name);

  if (args.Length() > 1 && args[0]->IsString() && args[1]->IsArray()) {
    int rval = 0;
    String::Utf8Value predicate(args[0]);

    if (obj->cb_log)
      obj->cb_log("Query::Open predicate: %s(", *predicate);
    Handle<Array> terms = Handle<Array>::Cast(args[1]);
    predicate_t p = PL_predicate(*predicate, terms->Length(), module_name);
    obj->term = PL_new_term_refs(terms->Length());
    obj->term_len = terms->Length();
    term_t t = obj->term;
    for (unsigned int i = 0; i < terms->Length(); i++) {
      Local<Value> v = terms->Get(i);
      if (v->IsInt32()) {
        if (obj->cb_log)
          obj->cb_log("%i", v->Int32Value());
        rval = PL_put_integer(t, v->Int32Value());
      } else if (v->IsNumber()) {
        if (obj->cb_log)
          obj->cb_log("%f", v->NumberValue());
        rval = PL_put_float(t, v->NumberValue());
      } else {
        String::Utf8Value s(v);
        if (obj->cb_log)
          obj->cb_log("%s", *s);
        rval = PL_chars_to_term(*s, t);
        int type = PL_term_type(t);
        if (obj->cb_log)
          obj->cb_log(" [%i]", type);
        switch (type) {
        case PL_VARIABLE:
          obj->varnames[t] = *s;
          break;
        case PL_ATOM:
        case PL_TERM:
        default:
          break;
        }
      }
      if (obj->cb_log)
        obj->cb_log(", ");
      t = t + 1;
    }

    obj->qid = PL_open_query(module, PL_Q_CATCH_EXCEPTION, p, obj->term);

    if (obj->cb_log)
      obj->cb_log(") #%li\n", obj->qid);

    if (obj->qid == 0) {
      isolate->ThrowException(
          Exception::Error(
              String::NewFromUtf8(isolate, "not enough space on the environment stack")));
      args.GetReturnValue().Set(Null(isolate));
    } else if (rval == 0) {
      isolate->ThrowException(
          Exception::Error(
              String::NewFromUtf8(isolate, GetExceptionString(PL_exception(obj->qid)))));
      args.GetReturnValue().Set(Null(isolate));
    } else {
      obj->open = OPEN;
      obj->Wrap(args.This());
      args.GetReturnValue().Set(args.This());
    }
  } else {
    isolate->ThrowException(
        Exception::SyntaxError(
            String::NewFromUtf8(isolate, "invalid arguments (pred, [ args ], module)")));
    args.GetReturnValue().Set(Null(isolate));
  }
}

void Query::NextSolution(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  int rval = 0;

  Query* obj = ObjectWrap::Unwrap<Query>(args.This());

  if (obj->cb_log)
    obj->cb_log("Query::NextSolution #%li", obj->qid);

  if (obj->open == OPEN) {
    rval = PL_next_solution(obj->qid);
    if (obj->cb_log)
      obj->cb_log(": %i\n", rval);

    if (rval) {
	  args.GetReturnValue().Set(ExportSolution(args, obj->term, obj->term_len, Object::New(isolate), obj->varnames));
    } else {
      args.GetReturnValue().Set(Boolean::New(isolate, false));
    }
  } else {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "query is closed")));
    args.GetReturnValue().Set(Null(isolate));
  }
}

void Query::Exception(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Query* obj = ObjectWrap::Unwrap<Query>(args.This());
  if (obj->cb_log)
    obj->cb_log("Query::Exception #%li\n", obj->qid);
  term_t term = PL_exception(obj->qid);

  if (term) {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, GetExceptionString(term))));
  } 
    
  args.GetReturnValue().Set(Boolean::New(isolate, false));
}

void Query::Close(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Query* obj = ObjectWrap::Unwrap<Query>(args.This());
  if (obj->open == OPEN) {
    if (obj->cb_log)
      obj->cb_log("Query::Close #%li\n", obj->qid);
    PL_close_query(obj->qid);
    obj->open = CLOSED;
  }

  args.GetReturnValue().Set(Boolean::New(isolate, true));
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "initialise", Initialise);
  NODE_SET_METHOD(exports, "term_type", TermType);
  NODE_SET_METHOD(exports, "cleanup", Cleanup);
  Query::Init(exports);
}

NODE_MODULE(addon, init)
