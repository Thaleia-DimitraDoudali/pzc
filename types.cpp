#include "types.h"

#include <iostream>

types_c::types_c (const std::string& nm):
	name (nm)
{
}

void
types_c::printType (Type type) {
	switch (type->type) {
		case TYPE_NONE:
			break;
		case TYPE_VOID:
			std::cout << "void";
			break;
		case TYPE_INTEGER:
			std::cout << "integer";
			break;
		case TYPE_BOOLEAN:
			std::cout << "boolean";
			break;
		case TYPE_CHAR:
			std::cout << "char";
			break;
		case TYPE_REAL:
			std::cout << "real";
			break;
		case TYPE_ARRAY:
			std::cout << "array [" << type->size << "] of ";
			printType(type->refType);
			break;
		case TYPE_IARRAY:
			//printf("array of ");
			//printType(type->refType);
			break;
		case NTYPES:
			break;
	}
}

variable_c::variable_c(std::string nm, Type tp) :
	types_c(nm),
	type (tp)
{
}

constant_c::constant_c(std::string nm, Type tp) :
	types_c(nm),
	type (tp)
{
}

parameter_c::parameter_c(std::string nm, Type tp, PassMode md) :
	types_c(nm),
	type (tp), mode(md)
{
}

function_c::function_c(std::string nm, bool is_forw, std::list<parameter_c *> par_list, Type tp) :
	types_c(nm),
	is_forward(is_forw), param_list(par_list), result_type (tp)
{
}

process_c::process_c(std::string nm, bool is_forw, std::list<parameter_c *> par_list) :
	types_c(nm),
	is_forward(is_forw), param_list(par_list)
{
}


void
variable_c::print() {
	std::cout << "[Variable] name: " << this->get_name() << " type: ";
	this->printType(this->get_type());
	std::cout << "\n";

}

void
constant_c::print() {
	std::cout << "[Constant] name: " << this->get_name() << " type: ";
	this->printType(this->get_type());
	std::cout << "\n";

}

void
parameter_c::print() {
	std::cout << "[Parameter] name: " << this->get_name() << " type: ";
	this->printType(this->get_type());
	std::cout << " pass mode: " << this->get_mode() << "\n";

}

void
function_c::print() {
	std::cout << "[Function] name: " << this->get_name() << " result type: ";
	this->printType(this->get_result_type());
	std::cout << " is forward: " << this->get_if_forward() << "\n";
	for (auto it = this->param_list.begin(); it != this->param_list.end(); ++it ){
		std::cout << "\t";
	   (*it)->print();
	}
	std::cout << "\n";
}

void
process_c::print() {
	std::cout << "[Process] name: " << this->get_name() << " is forward: "
			<< this->get_if_forward() << "\n";
	for (auto it = this->param_list.begin(); it != this->param_list.end(); ++it ){
		std::cout << "\t";
	   (*it)->print();
	}
	std::cout << "\n";
}








