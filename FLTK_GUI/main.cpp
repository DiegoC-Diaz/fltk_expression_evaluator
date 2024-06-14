#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Multiline_Output.H>
#include "Evaluator.h"
#include<iostream>
#include "Table.h"
/*
	Todo el coldigo comentado estara por eleminarse en siguientees versiones
*/

struct Linker {
	Fl_Multiline_Input* expression;
	Fl_Multiline_Output* output;
	Fl_Multiline_Output* postfix;
	Fl_Multiline_Output* history;
	Evaluator* evaluator; 

};

void display_hist_output(Fl_Multiline_Output * mlo,std::string output) {
	//std::string  curr_output = mlo->value();
	const char* out = (/*curr_output +*/ output).c_str();
	mlo->value(out);



}
void button_callback(Fl_Widget* widget, void* data) {
	Fl_Multiline_Input* input = (Fl_Multiline_Input*)data;
	Fl_Button* button = (Fl_Button*)widget;
	const char* current_txt = input->value();
	char result[100];   // array to hold the result.
	strcpy(result, current_txt); // copy string one into the result.
	strcat(result, button->label());
	input->value(result);


}



void eval_expression_callback(Fl_Widget* widget, void * data) {
	Linker* my_data = static_cast<Linker*>(data);
	// Access the data
	Fl_Multiline_Input* input = my_data->expression;
	Evaluator* evaluator = my_data->evaluator;


	//we need to add cusotm vairbales for user
// through the table....
// 
//	evaluator->loadUserConstants(table->g;et_data());
	std::string value = input->value();
	evaluator->inputExpression(value.c_str(), false);
	std::string prev = (my_data->history->value());

	my_data->history->value((prev + "\n" + value).c_str());


	std::string line="";
	//We could had  to clean teh output
	std::string output=evaluator->displayHistory();
	display_hist_output(my_data->output, output);


}

void assign_expression_callback(Fl_Widget* widget, void* data) {
	Linker* my_data = static_cast<Linker*>(data);

	// Access the data
	Fl_Multiline_Input* input = my_data->expression;
	Evaluator* evaluator = my_data->evaluator;
	Fl_Multiline_Output* variable_output = my_data->output;

	std::string value = input->value();
	evaluator->inputExpression(value.c_str(), true);

	std::string prev = (my_data->history->value());
	my_data->history->value((prev + "\n" + value).c_str());

	variable_output->value("");//We clean var output;

	std::string line = "";
	//We could had  to clean teh output
	for (auto it = evaluator->variables.begin(); it != evaluator->variables.end(); it++) {
		line = line + (it->first + "=" + std::to_string(it->second) + "\n");
		variable_output->value(line.c_str());
	}
	//std::string output = evaluator->displayHistory();
	//display_hist_output(my_data->output, output);


}
void show_button_callback(Fl_Widget* widget, void* data) {
	WidgetTable* table = (WidgetTable*)data;
	table->show_contents();


}
void grid(int x, Fl_Multiline_Input* in,Evaluator *evaluator){
	//pi, e, g, c
	Fl_Multiline_Output * out=new Fl_Multiline_Output(x, 20, 170, 20 * 4);
	std::string line;
	for (auto it = evaluator->constants.begin(); it != evaluator->constants.end(); it++) {
		line = line + (it->first + "=" + std::to_string(it->second) + "\n");
		out->value(line.c_str());
	}




}






const char* get_value(Fl_Multiline_Input* in) {
	return in->value();
}



int main(int argc, char** argv) {
	//Fl_Light_Button* lbutton = new Fl_Light_Button(x, y, width, height);
	Evaluator* evaluator = new Evaluator();
	Linker*link= new Linker();
	Linker* var_link = new Linker();
	//We link the evaluator with the struct Linker



	Fl_Window* window = new Fl_Window(600, 400);

	int x = 600;
	int y = 400;

	Fl_Multiline_Input* input_expression = new Fl_Multiline_Input((x / 2) - 100, 20, 200, 50, nullptr);
	Fl_Button* button_eval = new Fl_Button((x / 2) - 100, 20 + 60, 170, 20, "Evaluate");
	//We link the inut with Linke Struct.

	button_eval->type(FL_NORMAL_BUTTON);
	button_eval->callback(eval_expression_callback, link);
	Fl_Multiline_Output* out_put = new Fl_Multiline_Output((x / 2) - 100, (y / 2) - 100, 200, 390 - ((y / 2) - 100), nullptr);
	Fl_Multiline_Output* History =new Fl_Multiline_Output(x -200,20, 200, y-40, nullptr);
	grid(20, input_expression,evaluator);
	Fl_Multiline_Input* asignation_input = new Fl_Multiline_Input(20,100,170,20,"");
	Fl_Button* button_assing = new Fl_Button(20, 120, 170, 20, "Assign");
	button_assing->type(FL_NORMAL_BUTTON);
	button_assing->callback(assign_expression_callback, var_link);


	Fl_Multiline_Output* asignation_output = new Fl_Multiline_Output(20, 140, 170, y - 120);

	
	Fl_Button* button_show = new Fl_Button(x-170,y-20, 170, 20, "Show");


	link->history = History;
	link->evaluator = evaluator;
	link->output = out_put;
	link->expression = input_expression;

	var_link->evaluator = evaluator;
	var_link->output = asignation_output;
	var_link->history = History;
	var_link->expression = asignation_input;
	window->end();
	window->show(argc, argv);
	return Fl::run();
}