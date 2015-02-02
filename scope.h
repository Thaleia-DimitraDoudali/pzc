#ifndef _SCOPE_H_
#define _SCOPE_H_ 1

#include "types.h"

#include <unordered_map>
#include <string>
#include <list>

typedef std::unordered_map<std::string, types_c*> hash_table_t;


class scope_c
{
 public:

  static scope_c global_scope;
  static scope_c* current_scope;

  scope_c ();
  ~scope_c () {}


  void insert (std::string name, types_c *entry);
  bool lookup_current_scope (std::string name) ;
  types_c* lookup_curr_scope_ret(std::string name);
  static scope_c* get_current_scope() { return current_scope; }
  static void set_current_scope(scope_c* scp) { current_scope = scp; }
  static scope_c* get_global_scope() { return &global_scope; }
  scope_c* get_parent_scope() { return parent; }
  void add_child(scope_c* scp) { children.push_back(scp); }
  void print_hash_table();
  void print_scope(scope_c* scp);
  std::list<scope_c* > get_children() { return children; }
  void set_nesting_level(int nest_lvl) { nesting_level = nest_lvl; }
  int get_nesting_level() { return nesting_level; }

  //clear ();

 private:

  hash_table_t hash_table;
  scope_c* parent;
  int nesting_level;
  std::list<scope_c *> children;

};


#endif // _SCOPE_H_
