#include "parser_interface.h"
#include "types.h"
#include "scope.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <list>

using namespace std;

static list<parameter_c *> parameter_list;


void
new_parameter_list() {
   parameter_list.clear();
   return ;
}


void
new_parameter(const char* name, Type type, PassMode mode) {
  string nm = name;
  parameter_c *entry = new parameter_c(nm, type, mode);
  parameter_list.push_back(entry);
  return ;
}

void
new_function(const char* name, Type res_type, bool is_forw, int line) {
	std::string nm = name;
	scope_c *scope = scope_c::get_current_scope();
	if (is_forw == 1) {
		types_c *entry = new function_c(nm, is_forw, parameter_list,res_type);
		scope->insert(nm, entry);
	} else { //find the relevant entry and modify it + add parameters to current scope
			  std::string nm = name;
			  scope_c *scope = scope_c::get_current_scope();
			  function_c* look = (function_c*)scope->lookup_curr_scope_ret(nm);
			//If not forward then declaration - put function and parameters to symbol table
			if (look == NULL) {
				types_c *entry = new function_c(nm, is_forw, parameter_list,res_type);
				scope->insert(nm, entry);
				//parameters should be added to the child scope!!!
				std::list<scope_c *> children = scope->get_children();
				for (auto it = parameter_list.begin(); it != parameter_list.end(); ++it ){
					children.back()->insert((*it)->get_name(), (*it));
				}
			} else {
				//check result type
				std::cout << res_type->type << " " << look->get_result_type()->type;
				if (res_type->type != look->get_result_type()->type) {
					std::cout << "Line: " << line << " error: conflicting types for " << nm << "\n";
					exit(1);
				}
				//check number of parameters
				std::list<parameter_c *> par_list = look->get_param_list();
				if (par_list.size() != parameter_list.size()) {
					std::cout << "Line: " << line << " error: conflicting types for " << nm << "\n";
					exit(1);
				}
				//check each parameter one by one
				auto iter = par_list.begin();
				for (auto it = parameter_list.begin(); it != parameter_list.end(); ++it ){
					if ( ((*it)->get_type()->type != (*iter)->get_type()->type) ||
							((*it)->get_mode() != (*iter)->get_mode())) {
						std::cout << "Line: " << line << " error: conflicting types for " << nm << "\n";
						exit(1);
					} else {
						//if array check refType and size
						if ((*it)->get_type()->type == TYPE_ARRAY) {
							if (((*it)->get_type()->refType->type != (*iter)->get_type()->refType->type) ||
									((*it)->get_type()->size != (*iter)->get_type()->size)) {
								std::cout << "Line: " << line << " error: conflicting types for " << nm << "\n";
								exit(1);
							}
						}
					}
					++iter;
				}
				//all checks ok: modify symbol table entry, add parameters to symbol table
				look->set_forward(false);

				//parameters should be added to the child scope!!!
				std::list<scope_c *> children = scope->get_children();
				for (auto it = parameter_list.begin(); it != parameter_list.end(); ++it ){
					if (!children.empty())
						children.back()->insert((*it)->get_name(), (*it));
				}

			}

		}

	return;
}

void
new_process(const char* name, bool is_forw, int line) {
	std::string nm = name;
	scope_c *scope = scope_c::get_current_scope();
	if (is_forw == 1) {
		types_c *entry = new process_c(nm, is_forw, parameter_list);
		scope->insert(nm, entry);
	} else { //find the relevant entry and modify it + add parameters to current scope
		  std::string nm = name;
		  scope_c *scope = scope_c::get_current_scope();
		  process_c* look = (process_c*)scope->lookup_curr_scope_ret(nm);
		//If not forward then declaration - put function and parameters to symbol table
		if (look == NULL) {
			types_c *entry = new process_c(nm, is_forw, parameter_list);
			scope->insert(nm, entry);
			//parameters should be added to the child scope!!!
			std::list<scope_c *> children = scope->get_children();
			for (auto it = parameter_list.begin(); it != parameter_list.end(); ++it ){
				children.back()->insert((*it)->get_name(), (*it));
			}
		} else {
			//check number of parameters
			std::list<parameter_c *> par_list = look->get_param_list();
			if (par_list.size() != parameter_list.size()) {
				std::cout << "Line: " << line << " error: conflicting types for " << nm << "\n";
				exit(1);
			}
			//check each parameter one by one
			auto iter = par_list.begin();
			for (auto it = parameter_list.begin(); it != parameter_list.end(); ++it ){
				if ( ((*it)->get_type()->type != (*iter)->get_type()->type) ||
						((*it)->get_mode() != (*iter)->get_mode())) {
					std::cout << "Line: " << line << " error: conflicting types for " << nm << "\n";
					exit(1);
				} else {
					//if array check refType and size
					if ((*it)->get_type()->type == TYPE_ARRAY) {
						if (((*it)->get_type()->refType->type != (*iter)->get_type()->refType->type) ||
								((*it)->get_type()->size != (*iter)->get_type()->size)) {
							std::cout << "Line: " << line << " error: conflicting types for " << nm << "\n";
							exit(1);
						}
					}
				}
				++iter;
			}
			//all checks ok: modify symbol table entry, add parameters to symbol table
			look->set_forward(false);
			//parameters should be added to the child scope!!!
			std::list<scope_c *> children = scope->get_children();
			for (auto it = parameter_list.begin(); it != parameter_list.end(); ++it ){
				children.back()->insert((*it)->get_name(), (*it));
			}
		}
	}
	return;
}


void
new_variable(const char* name, Type type) {
	std::string nm = name;
	types_c *entry = new variable_c(nm, type);
	scope_c *scope = scope_c::get_current_scope();
	scope->insert(nm, entry);
	return;
}

void
new_constant(const char* name, Type type) {
	std::string nm = name;
	types_c *entry = new constant_c(nm, type);
	scope_c *scope = scope_c::get_current_scope();
	scope->insert(nm, entry);
  return;
}

Type
typeArray (RepInteger size, Type refType)
{
    Type n = (Type) std::malloc(sizeof(struct Type_tag));

    //TODO:fatal error
    //if (n == NULL)
        //fatal("Out of memory");

    n->type     = TYPE_ARRAY;
    n->refType  = refType;
    n->size     = size;
    n->refCount = 0;

    return n;
}

Type
typeIArray (Type refType)
{
    Type n = (Type) std::malloc(sizeof(struct Type_tag));

    n->type     = TYPE_IARRAY;
    n->refType  = refType;
    n->refCount = 0;

    return n;
}

bool
check_array_index(Type_Value tpvl, int line) {
	if (tpvl->type->type != TYPE_INTEGER) {
		std::cout << "Line: " << line << " error: array index must be integer\n";
		return false;
	//TODO:check how to identify negatives
	} else if (tpvl->val.vInteger <= 0) {
		std::cout << "Line: " << line << " error: array index must be positive\n";
		return false;
	}
	return true;
}


bool 
lookup(const char* name) {
  std::string nm = name;
  /*Search in current scope's hashtable the entry with that name*/
  scope_c *scope = scope_c::get_current_scope();
  return scope->lookup_current_scope(nm);
}


bool
lookup_all_scopes(const char* name) {
	bool lookup = false, res = false;
	std::string nm = name;
	scope_c *scope = scope_c::get_current_scope();
	while (!lookup) {
		lookup = scope->lookup_current_scope(nm);
		res = lookup;
		scope = scope->get_parent_scope();
		if (scope == NULL)  {
			lookup = true;
		}
	}
	return res;
}


void 
open_scope() { 
  std::cout << "Opening scope." << '\n';
  /*Create scope object*/
  scope_c *scope = new scope_c();
  scope_c::set_current_scope(scope);
  scope_c *parent_scope = scope->get_parent_scope();
  parent_scope->add_child(scope);
  int nesting_lvl = parent_scope->get_nesting_level();
  nesting_lvl ++;
  scope->set_nesting_level(nesting_lvl);
  return;
}
void
open_global_scope() {
	std::cout << "Opening global scope." << '\n';
	/*Create scope object*/

	scope_c *scope = scope_c::get_global_scope();
	scope_c::set_current_scope(scope);
	scope->set_nesting_level(0);
	//scope_c *parent_scope = scope->get_parent_scope();
	//scope->add_child(parent_scope);
	return;
}

void 
close_scope() { 
  std::cout << "Closing scope." << '\n';
  scope_c *current_scope = scope_c::get_current_scope();
  scope_c *parent_scope = current_scope->get_parent_scope();
  scope_c::set_current_scope(parent_scope);
  return;
}

void
print_all_scopes() {
	scope_c *scope = scope_c::get_global_scope();
	scope->print_scope(scope);
}

















