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

struct Linker {
	Fl_Multiline_Input* expression;
	Fl_Multiline_Output* output;
	Evaluator* evaluator;  // Replace SomeClass with your actual class


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
	evaluator->inputExpression(input->value());
	std::string output=evaluator->displayHistory();
	display_hist_output(my_data->output, output);

}
void show_button_callback(Fl_Widget* widget, void* data) {
	WidgetTable* table = (WidgetTable*)data;
	table->show_contents();


}
void grid(int x, Fl_Multiline_Input* in) {
	//pi, e, g, c
	Fl_Button* button_pi = new Fl_Button(x, 20, 170, 20, "pi");
	Fl_Button* button_e = new Fl_Button(x, 20 + 20, 170, 20, "e");
	Fl_Button* button_g = new Fl_Button(x, 20 + 40, 170, 20, "g");
	Fl_Button* button_c = new Fl_Button(x, 20 + 60, 170, 20, "c");
	
	button_pi->type(FL_NORMAL_BUTTON);
	button_g->type(FL_NORMAL_BUTTON);
	button_e->type(FL_NORMAL_BUTTON);
	button_c->type(FL_NORMAL_BUTTON);
	button_pi->callback(button_callback, in);
	button_e->callback(button_callback, in);
	button_g->callback(button_callback, in);
	button_c->callback(button_callback, in);
}






const char* get_value(Fl_Multiline_Input* in) {
	return in->value();
}



int main(int argc, char** argv) {
	//Fl_Light_Button* lbutton = new Fl_Light_Button(x, y, width, height);
	Evaluator* evaluator = new Evaluator();
	Linker*link= new Linker();
	//We link the evaluator with the struct Linker
	link->evaluator = evaluator;

	Fl_Window* window = new Fl_Window(600, 400);

	int x = 600;
	int y = 400;

	Fl_Multiline_Input* input_expression = new Fl_Multiline_Input((x / 2) - 100, 20, 200, 50, nullptr);
	Fl_Button* button_eval = new Fl_Button((x / 2) - 100, 20 + 60, 170, 20, "Evaluate");
	//We link the inut with Linke Struct.
	link->expression = input_expression;
	//attacth the our eval_expression_callback to the button
	button_eval->type(FL_NORMAL_BUTTON);
	button_eval->callback(eval_expression_callback, link);
	Fl_Multiline_Output* out_put = new Fl_Multiline_Output((x / 2) - 100, (y / 2) - 100, 200, 390 - ((y / 2) - 100), nullptr);
	//Atttach output to Linker Struct
	link->output = out_put;

	grid(20, input_expression);
	WidgetTable * table =callTable(20, 120);

	//Test Display Varaible stored Button
	Fl_Button* button_show = new Fl_Button(x-170,y-40, 170, 20, "Show");
	button_show->type(FL_NORMAL_BUTTON);
	button_show->callback(show_button_callback, table);



	window->end();
	window->show(argc, argv);
	return Fl::run();
}