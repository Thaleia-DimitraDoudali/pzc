#include "scope.h"

#include <string>
#include <iostream>

scope_c *scope_c::current_scope = NULL;
scope_c scope_c::global_scope = scope_c() ;

scope_c::scope_c() :
	parent(current_scope)
{
}

void scope_c::insert (std::string name, types_c *entry) {
	hash_table[name] = entry;
}

bool scope_c::lookup_current_scope (std::string name) {
	auto it = hash_table.find(name);
	if (it != hash_table.end())
		return true;
	else
		return false;
}

types_c*
scope_c::lookup_curr_scope_ret(std::string name) {
	auto it = hash_table.find(name);
	if (it != hash_table.end())
		return (it->second);
	else
		return NULL;
}

void
scope_c::print_hash_table() {

	for (auto it = hash_table.begin(); it != hash_table.end(); ++it ){
		it->second->print();
	}
	return;
}

void
scope_c::print_scope(scope_c* scope) {
	std::list<scope_c* > children_scopes = scope->get_children();
	if (!children_scopes.empty()) {
		for (auto it = children_scopes.begin(); it != children_scopes.end(); ++it ){
			print_scope(*it);
		}
	}
	std::cout << "<Scope> " << scope->get_nesting_level() << "\n";
	scope->print_hash_table();
}

// void hashTable_clear()
// {
//     hashTable.clear();
// }

// void hashTable_forall(void (*callback)(void*))
// {
//     for (auto namegroup: hashTable)
//         for (auto entry: namegroup.second)
//             callback(entry);
// }
// 
// void hashTable_insert(const char* name, void* entry)
// {
//     hashTable[name].push_back(entry);
// }

// void hashTable_remove(const char* name)
// {
//     auto it = hashTable.find(name);
//     if (it == hashTable.end()) return;
//     if (!it->second.empty()) it->second.pop_back();
//     if (it->second.empty()) hashTable.erase(it);
// }
// 
// void* hashTable_lookup(const char * name)
// {
//     auto it = hashTable.find(name);
//     if (it == hashTable.end() || it->second.empty()) return nullptr;
//     return it->second.back();
// }

