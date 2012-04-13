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
#include <SWI-Prolog.h>

using namespace v8;
using namespace node;

Handle<Value> Initialise(const Arguments& args);

Handle<Value> TermType(const Arguments& args);

Handle<Value> Cleanup(const Arguments& args);

class Query : public node::ObjectWrap {
 public:
  static const int OPEN = 1;
  static const int CLOSED = 0;
  static void Init(v8::Handle<v8::Object> target);

 private:
  Query();
  ~Query();

  static v8::Handle<v8::Value> Open(const v8::Arguments& args);
  static v8::Handle<v8::Value> NextSolution(const v8::Arguments& args);
  static v8::Handle<v8::Value> Close(const v8::Arguments& args);
  static v8::Handle<v8::Value> Exception(const v8::Arguments& args);
  int open;
  qid_t qid;
  term_t term;
  Persistent<Object> varnames;
  int term_len;
  int (*cb_log)(const char*, ...);
};

#endif /* NODE_LIBSWIPL_H_ */
