#ifndef _TYPES_H_
#define _TYPES_H_ 1

#include "parser_interface.h"

#include <string>
#include <list>

class types_c
{
	public:
		types_c (const std::string& nm);
		virtual ~types_c() {}
		virtual EntryType get_entry_type() = 0;
		virtual void print() = 0;
		void printType(Type type);
		std::string get_name() { return name; }
	private:
		std::string name;
};

class variable_c : public types_c
{
	public:
		variable_c (std::string nm, Type tp);
		EntryType get_entry_type() { return ENTRY_VARIABLE; }
		void print();
		Type get_type() { return type; }
	private:
		Type type;
};

class constant_c : public types_c
{
	public:
		constant_c (std::string nm, Type tp);
		EntryType get_entry_type() { return ENTRY_VARIABLE; }
		void print();
		Type get_type() { return type; }
	private:
		Type type;
		Value val;
};

class parameter_c : public types_c
{
	public:
		parameter_c (std::string nm, Type tp, PassMode md);
		EntryType get_entry_type() { return ENTRY_PARAMETER; }
		void print();
		Type get_type() { return type; }
		PassMode get_mode() { return mode;}
	private:
		Type type;
		PassMode mode;
};

class function_c : public types_c
{
	public:
		function_c (std::string nm, bool is_forw, std::list<parameter_c *> par_list, Type tp);
		EntryType get_entry_type() { return ENTRY_FUNCTION;}
		void print();
		Type get_result_type() { return result_type; }
		bool get_if_forward() { return is_forward; }
		void set_forward(bool forw) {is_forward = forw;}
		std::list<parameter_c *> get_param_list() { return param_list; }
	private:
		bool is_forward;
		std::list<parameter_c *> param_list;
		Type result_type;
};

class process_c : public types_c
{
	public:
		process_c (std::string nm, bool is_forw, std::list<parameter_c *> par_list);
		EntryType get_entry_type() { return ENTRY_PROCESS;}
		void print();
		bool get_if_forward() { return is_forward; }
		void set_forward(bool forw) {is_forward = forw;}
		std::list<parameter_c *> get_param_list() { return param_list; }
	private:
		bool is_forward;
		std::list<parameter_c *> param_list;
};













#endif // _TYPES_H_
