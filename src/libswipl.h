/*  $Id$

    node.js addon for SWI-Prolog

    Author:        Tom Klonikowski
    E-mail:        klonik_t@informatik.haw-hamburg.de
    Copyright (C): 2012, Tom Klonikowski

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef NODE_LIBSWIPL_H_
#define NODE_LIBSWIPL_H_

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <SWI-Prolog.h>
#include "v8-util.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace v8;
using namespace node;
using namespace std;

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Persistent;
using v8::Value;
using v8::Null;
using v8::HandleScope;

class Query : public node::ObjectWrap {
 public:
  static const int OPEN = 1;
  static const int CLOSED = 0;
  static void Init(Local<Object> exports);

 private:
  explicit Query();
  ~Query();

  static void New(const FunctionCallbackInfo<Value>& args);
  static void Open(const FunctionCallbackInfo<Value>& args);
  static void NextSolution(const FunctionCallbackInfo<Value>& args);
  static void Close(const FunctionCallbackInfo<Value>& args);
  static void Exception(const FunctionCallbackInfo<Value>& args);

  static v8::Persistent<v8::Function> constructor;
  int open;
  qid_t qid;
  term_t term;
  map<int, std::string> varnames;
  int term_len;
  int (*cb_log)(const char*, ...);
};

#endif /* NODE_LIBSWIPL_H_ */
