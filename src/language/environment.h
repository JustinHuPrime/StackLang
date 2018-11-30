// Copyright 2018 Justin Hu, Bronwyn Damm, Jacques Marais, Ramon Rakow, and Jude
// Sidloski
//
// This file is part of the StackLang interpreter.
//
// The StackLang interpreter is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// The StackLang interpreter is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// the StackLang interpreter.  If not, see <https://www.gnu.org/licenses/>.

// environment tree

#ifndef STACKLANG_LANGUAGE_ENVIRONMENT_H_
#define STACKLANG_LANGUAGE_ENVIRONMENT_H_

#include "language/stack/stack.h"

#include <map>
#include <vector>

namespace stacklang {
class EnvTree {
 public:
  class Environment {
   public:
    std::map<std::string, StackElement*> bindings;
    Environment* parent;

    Environment(Environment*) noexcept;
    Environment(Environment&&) = default;

    ~Environment() noexcept;

    Environment& operator=(Environment&&) = default;

    StackElement* lookup(const std::string&);
    void clearBindings() noexcept;

   private:
    std::vector<Environment*> children;
  };

  EnvTree() noexcept;
  EnvTree(EnvTree&&) = default;

  ~EnvTree() noexcept;

  EnvTree& operator=(EnvTree&&) = default;

  Environment* getRoot() noexcept;

 private:
  Environment* root;
};

typedef EnvTree::Environment Environment;
}  // namespace stacklang

#endif