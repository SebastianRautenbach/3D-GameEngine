#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>


/*
				  ____________ _____
				 |___  /  ____|  __ \
					/ /| |__  | |__) |
				   / / |  __| |  _  /
				_ / /__| |____| | \ \
			   (_)_____|______|_|  \_\


			 Sebastian Rautenbach
			 ~~~~~~~~~~~~~~~~~~~~~


			 Apache License 2.0
			 ~~~~~~~~~~~~~~~~~~

			 All source and Images: Copyright 2023

			 Redistribution and use in source and binary forms, with or without
			 modification, are permitted provided that the following conditions
			 are met:

			 1. Redistributions or derivations of source code must retain the above
			 copyright notice, this list of conditions and the following disclaimer.

			 2. Redistributions or derivative works in binary form must reproduce
			 the above copyright notice. This list of conditions and the following
			 disclaimer must be reproduced in the documentation and/or other
			 materials provided with the distribution.

			 3. Neither the name of the copyright holder nor the names of its
			 contributors may be used to endorse or promote products derived
			 from this software without specific prior written permission.

			 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
			 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
			 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
			 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
			 HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
			 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
			 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
			 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
			 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
			 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
			 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


*/

// Simple serializer to save any project





namespace filedata
{
	/*
		These data structures can be easily identified by looking for their var_name
		identifier
	*/




	class ZER {
	public:
		std::map<std::string, ZER*> class_properties;
		bool read_file_success = false;

	public:


		// Default constructors 
		ZER(std::string id) {
			property_id = id;
		}
		ZER() {

		}
		~ZER() {
			for (auto& i : class_properties) {
				delete i.second;
			}
		}

		ZER& c_class_struct(std::string id) {
			if (class_properties[id] != nullptr)
				return *class_properties[id];
			class_properties[id] = new ZER(id);
			return *class_properties[id];
		}


		// Remove the varname 

		void remove_var(ZER& property_ref, std::string var_name) {

			for (const auto& i : property_ref.variables)
			{
				if (i.find(var_name) != -1)
				{
					property_ref.variables.erase(std::remove(property_ref.variables.begin(), property_ref.variables.end(), i), property_ref.variables.end());
				}
			}

			return;
		}


		// if there is any char over 

		void remove_non_ascii_char(std::string& str) {
			std::string concat_str;
			for (auto& i : str)
			{
				if (static_cast<unsigned char>(i) <= 127)
					concat_str += i;
			}
			str = concat_str;
		}



		//the vector int value is there if you want to parse a array in the save file
		//this function only adds the var name and value to a string for further

		void set_int(std::string var_name, std::vector<int> value) {
			bool value_does_exist = false;
			std::string concat_string;

			if (value.size() == 0)
			{
				remove_var(*this, var_name);
				return;
			}

			//first lets find the variable if it exists
			for (auto& i : variables)
			{
				if (i.find(var_name) != -1) {

					// formulate all the variable data in a neat string
					for (auto i : value)
					{
						concat_string += std::to_string(i) + ",";
					}

					concat_string.pop_back();
					concat_string += ';';
					value_does_exist = true;
					i = var_name + ":" + concat_string;
				}
			}
			// if the variable does not exist then create one
			if (!value_does_exist)
			{
				std::string concat_string = var_name + ":";
				for (auto i : value)
				{
					concat_string += std::to_string(i) + ",";
				}
				concat_string.pop_back();
				concat_string += ';';
				variables.push_back(concat_string);
			}
		}

		void set_float(std::string var_name, std::vector<float> value) {

			bool value_does_exist = false;
			std::string concat_string;

			if (value.size() == 0)
			{
				remove_var(*this, var_name);
				return;
			}


			//first lets find the variable if it exists
			for (auto& i : variables)
			{
				if (i.find(var_name) != -1) {

					// formulate all the variable data in a neat string
					for (auto i : value)
					{
						concat_string += std::to_string(i) + ",";
					}

					remove_non_ascii_char(concat_string);

					value_does_exist = true;
					concat_string.pop_back();
					concat_string += ';';
					i = var_name + ":" + concat_string;
				}
			}
			// if the variable does not exist then create one
			if (!value_does_exist)
			{
				std::string concat_string = var_name + ":";
				for (auto i : value)
				{
					concat_string += std::to_string(i) + ",";
				}

				remove_non_ascii_char(concat_string);

				concat_string.pop_back();
				concat_string += ';';
				variables.push_back(concat_string);
			}
		}

		void set_string(std::string var_name, std::vector<std::string> value) {

			bool value_does_exist = false;
			std::string concat_string;

			if (value.size() == 0)
			{
				remove_var(*this, var_name);
				return;
			}


			//first lets find the variable if it exists
			for (auto& i : variables)
			{
				if (i.find(var_name) != -1) {

					// formulate all the variable data in a neat string
					for (const auto& i : value)
					{

						concat_string += i + ",";
					}

					remove_non_ascii_char(concat_string);

					value_does_exist = true;
					concat_string.pop_back();
					concat_string += ';';
					i = var_name + ":" + concat_string;
				}
			}
			// if the variable does not exist then create one
			if (!value_does_exist)
			{
				std::string concat_string = var_name + ":";
				for (const auto& i : value)
				{
					concat_string += i + ",";
				}


				remove_non_ascii_char(concat_string);

				concat_string.pop_back();
				concat_string += ';';
				variables.push_back(concat_string);
			}
		}


		// get functions

		std::vector<int> get_int(std::string var_name) {

			std::string variable_value;
			std::string makeup_var;
			std::vector<int> return_val;

			for (const auto& i : this->variables)
			{
				int index_found_at = i.find(var_name);
				if (index_found_at != -1)
				{
					index_found_at += var_name.size();
					variable_value = retrieve_right_operand(i, index_found_at) + ","; break;
				}
			}
			for (const char i : variable_value)
			{
				if (i == ',') {
					return_val.push_back(std::stoi(makeup_var));
					makeup_var.clear();
				}
				else
				{
					makeup_var += i;
				}
			}

			if (return_val.empty()) {
				return {0};
			}

			return return_val;
		}
		std::vector<float> get_float(std::string var_name) {
			std::string variable_value;
			std::string makeup_var;
			std::vector<float> return_val;

			for (auto i : this->variables)
			{
				int index_found_at = i.find(var_name);
				if (index_found_at != -1)
				{
					index_found_at += var_name.size();
					variable_value = retrieve_right_operand(i, index_found_at) + ","; break;
				}
			}
			for (const char i : variable_value)
			{
				if (i == ',') {
					return_val.push_back(std::stof(makeup_var));
					makeup_var.clear();
				}
				else
				{
					makeup_var += i;
				}
			}
			if (return_val.empty()) {
				return { 0.0 };
			}

			return return_val;
		}
		std::vector<std::string> get_string(std::string var_name) {
			std::string variable_value;
			std::string makeup_var;
			std::vector<std::string> return_val;

			for (auto i : this->variables)
			{
				int index_found_at = i.find(var_name);
				if (index_found_at != -1)
				{
					index_found_at += var_name.size();
					variable_value = retrieve_right_operand(i, index_found_at) + ","; break;
				}
			}
			for (const char i : variable_value)
			{
				if (i == ',') {
					return_val.push_back(makeup_var);
					makeup_var.clear();
				}
				else
				{
					makeup_var += i;
				}
			}

			if (return_val.empty()) {
				return { ""};
			}

			return return_val;
		}

	public:
		// iterate through this class's properties and variables
		void read_class_properties(ZER& property_ref, std::string& file_save_data, int& crnt_indentation) {

			for (const auto& i : property_ref.variables)
			{
				// give correct indentation
				for (int i = 0; i < crnt_indentation + 1; i++)
					file_save_data += "\t";

				file_save_data += i + "\n";
			}
			for (const auto& i : property_ref.class_properties)
			{
				++crnt_indentation;

				for (int i = 0; i < crnt_indentation; i++)
					file_save_data += "\t";

				file_save_data += i.second->property_id + "{\n";
				read_class_properties(*i.second, file_save_data, crnt_indentation);

				for (int i = 0; i < crnt_indentation + 1; i++)
					file_save_data += "\t";

				file_save_data += "}\n";
			}

			// make sure when structure is complete to remove one indent

			--crnt_indentation;
		}






		void save_file(ZER& property_ref, std::string filename = "save.zer") {
			std::ofstream save_to_file(filename);
			std::string file_save_data;


			std::erase(file_save_data, 9);
			int indentation = -1;
			read_class_properties(property_ref, file_save_data, indentation);


			save_to_file << file_save_data;
			save_to_file.close();
		}

		// retrieve the name on the left side just before the '\n'
		std::string retrieve_property_name(const std::string& data, unsigned int starting_index) {
			int ittr = starting_index - 1;
			std::string constructed_name;
			std::string reconstructed_name;
			while (ittr != -1 && data[ittr] != '\n')
			{
				constructed_name += data[ittr];
				--ittr;
				if (ittr == -1)
					break;
			}
			for (unsigned int i = constructed_name.size() - 1; i + 1 > 0; i--)
			{
				reconstructed_name += constructed_name[i];
			}
			return reconstructed_name;
		}

		// retrieve the value on the right side just before the '\n'
		std::string retrieve_right_operand(const std::string& data, int& starting_index) {
			std::string constructed_val;
			int ittr = starting_index + 1;
			while (data[ittr] != ';')
			{
				constructed_val += data[ittr];
				++ittr;
			}
			starting_index = ittr;
			return constructed_val;
		}



		void construct_class_from_file(std::string& data, ZER& root, int& starting_index) {

			// make sure to clear all indentation
			std::erase(data, 9);

			for (int i = starting_index; i < data.size(); i++)
			{
				// first we need to start another branch when we see this { in order to add value to the new branch

				if (data[i] == '{') {
					std::string name_of_property = retrieve_property_name(data, i);
					++i;
					construct_class_from_file(data, root[name_of_property], i);
				}

				// Second lets add variables into the string when we see the :
				if (data[i] == ':') {
					std::string name_of_variable = retrieve_property_name(data, i);
					std::string line_of_variable = retrieve_right_operand(data, i);
					root.set_string(name_of_variable, { line_of_variable });
				}

				//lastly exit the current branch into its root in order to continue
				//because this branch is complete
				if (data[i] == '}')
				{
					starting_index = i + 1; return;
				}
			}
		}

		void read_file_cntx(const std::string file_name = "save.zer") {
			std::string file_data, line_data;
			std::ifstream read_file(file_name);

			if (read_file.is_open()) {
				read_file_success = true;
				while (std::getline(read_file, line_data))
					file_data += line_data + '\n';
				read_file.close();

				int start_index = 0;
				std::erase(file_data, 9);
				construct_class_from_file(file_data, *this, start_index);
			}

		
			if (!read_file_success) {
				std::cerr << "Failed to read file: " << file_name << std::endl;
			}
		}

		void read_string_cntx(std::string string_data) {
			int start_index = 0;
			std::erase(string_data, 9);
			construct_class_from_file(string_data, *this, start_index);
		}


		/*

			This operator overload makes it simpler to use by calling ["foo"]
			instead of ZER foo.c_class_struct.c_class_struct ect...

			The id
		*/

		ZER& operator[] (std::string id) {
			if (class_properties[id] != nullptr)
				return *this->class_properties[id];
			class_properties[id] = new ZER(id);
			return *class_properties[id];
		}

	private:
		std::string property_id;
		std::vector<std::string> variables;

	};

}