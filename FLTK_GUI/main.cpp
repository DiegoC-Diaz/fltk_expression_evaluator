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
#include "evaluator.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
/*
	Todo el coldigo comentado estara por eleminarse en siguientees versiones
*/
TEST_CASE("Funciones infijas a postfijas", "InfixPostFix") {

	SECTION("Infija a Postfija") {

		Evaluator evaluator;

		std::string tokens = "3 + 4";
		std::string tokens1 = "5 - 2";
		std::string tokens2 = "6 * 7";
		std::string tokens3 = "8 / 4";
		std::string tokens4 = "2 ^ 3";
		std::string tokens5 = "3 + 4 * 5";
		std::string tokens6 = "( 3 + 4 ) * 5";
		std::string tokens7 = "6 * ( 7 + 8 )";
		std::string tokens8 = "9 / ( 2 + 3 )";
		std::string tokens9 = "( 4 + 5 ) / ( 6 - 3 )";
		//Aqui empiezan los malos
		std::string tokens10 = "3 + 4 - )";
		std::string tokens11 = "( 6 + 7 * ( 8 - 4 )";
		std::string tokens12 = "5 + ( 6 * 7 ) - ";
		std::string tokens13 = "4 + ( 9 / )";
		std::string tokens14 = "( 10 - 2 ) / ";
		std::string tokens15 = "3 + 4 * - 6 / 2";
		std::string tokens16 = "7 * ( 3 + 5 / 2";
		std::string tokens17 = "( 6 + * 3 ) / ( 8 - 4 )";
		std::string tokens18 = "2 * 3 + * 5";
		std::string tokens19 = "( 1 + 2 ) * + 4 )";



		/* Estsos son los casos Correctos
		std::string tokens10= "3 + 4 - 5";
		std::string tokens11= "( 6 + 7 ) * ( 8 - 4 )";
		std::string tokens12= "5 + ( 6 * 7 ) - 8";
		std::string tokens13= "4 + ( 9 / 3 )";
		std::string tokens14= "( 10 - 2 ) / 4";
		std::string tokens15 = "3 + 4 * 5 - 6 / 2";
		std::string tokens16= "7 * ( 3 + 5 ) / 2";
		std::string tokens17 = "( 6 + 2 * 3 ) / ( 8 - 4 )";
		std::string tokens18 = "2 * 3 + 4 * 5";
		std::string tokens19 = "( 1 + 2 ) * ( 3 + 4 )";

	   */
		CHECK(evaluator.inputExpression(tokens, false) == true);
		CHECK(evaluator.inputExpression(tokens1, false) == true);
		CHECK(evaluator.inputExpression(tokens2, false) == true);
		CHECK(evaluator.inputExpression(tokens3, false) == true);
		CHECK(evaluator.inputExpression(tokens4, false) == true);
		CHECK(evaluator.inputExpression(tokens5, false) == true);
		CHECK(evaluator.inputExpression(tokens6, false) == true);
		CHECK(evaluator.inputExpression(tokens7, false) == true);
		CHECK(evaluator.inputExpression(tokens8, false) == true);
		CHECK(evaluator.inputExpression(tokens9, false) == true);

		CHECK(evaluator.inputExpression(tokens10, false) == true);
		CHECK(evaluator.inputExpression(tokens11, false) == true);
		CHECK(evaluator.inputExpression(tokens12, false) == true);
		CHECK(evaluator.inputExpression(tokens13, false) == true);
		CHECK(evaluator.inputExpression(tokens14, false) == true);
		CHECK(evaluator.inputExpression(tokens15, false) == true);
		CHECK(evaluator.inputExpression(tokens16, false) == true);
		CHECK(evaluator.inputExpression(tokens17, false) == true);
		CHECK(evaluator.inputExpression(tokens18, false) == true);
		CHECK(evaluator.inputExpression(tokens19, false) == true);
	}
}
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
	if (value == "") {
		return;
	}
	evaluator->inputExpression(value.c_str(), false);
	std::string prev_hist= (my_data->history->value());


	my_data->history->value((prev_hist+value+"\n").c_str());

	std::string line="";
	for (std::string it:evaluator->postfixHistory) {
		std::cout << line;
		line = line +it+"\n";
		
	}
	my_data->postfix->value(line.c_str());

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
	if (value == "" || value.back() == '=') {


		MessageBox(NULL, L"Invalid assign operation!", L"Error Setting Variable", MB_ICONERROR | MB_OK);

		
		return;
	}
	evaluator->inputExpression(value.c_str(), true);

	std::string prev = (my_data->history->value());
	my_data->history->value((prev+value+"\n").c_str());

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

void grid(int x, Fl_Multiline_Input* in,Evaluator *evaluator){
	//pi, e, g, c
	Fl_Multiline_Output * out=new Fl_Multiline_Output(x, 20, 200, 20 * 4);
	std::string line;
	for (auto it = evaluator->constants.begin(); it != evaluator->constants.end(); it++) {
		line = line + (it->first + "=" + std::to_string(it->second) + "\n");
		out->value(line.c_str());
	}

}






const char* get_value(Fl_Multiline_Input* in) {
	return in->value();
}

void drawText(int x , int y, int w, int h,const char* text) {

	Fl_Label* label2 = new Fl_Label();
	Fl_Box* box2 = new Fl_Box(x,y,w,h, text);
	box2->labelsize(20);
	box2->labelfont(FL_SCREEN);

}



int main(int argc, char* argv[]) {
	//Fl_Light_Button* lbutton = new Fl_Light_Button(x, y, width, height);
	Evaluator* evaluator = new Evaluator();
	Linker* link = new Linker();
	Linker* var_link = new Linker();
	//We link the evaluator with the struct Linker



	Fl_Window* window = new Fl_Window(800, 500);

	int x = 800;
	int y = 500;
	int widths = 200;

	//Creating  Text Inputs and Otpus
	Fl_Multiline_Output* out_put = new Fl_Multiline_Output((x / 2) - 100, (y / 2) - 150, widths, y / 2 - 100 + 50, nullptr);
	Fl_Multiline_Output* post_out_put = new Fl_Multiline_Output((x / 2) - 100, (y / 2) + 70, widths, y / 2 - 100, nullptr);
	Fl_Multiline_Output* History = new Fl_Multiline_Output(x - 200, 20, widths, y - 40, nullptr);
	Fl_Multiline_Output* asignation_output = new Fl_Multiline_Output(20, 140, widths, y - 120);
	Fl_Multiline_Input* asignation_input = new Fl_Multiline_Input(20, 100, widths, 20, "");
	Fl_Multiline_Input* input_expression = new Fl_Multiline_Input((x / 2) - 100, 20, widths, 50, nullptr);

	drawText((x / 2) - 100, (y / 2) + 50, widths, 20, "Post Fix");
	drawText(0, 0, x, 20, "Constants\t\tEvaluator\t\tHistory");




	//box->labeltype(FL_FREE_LABELTYPE);

	Fl_Button* button_eval = new Fl_Button((x / 2) - 100, 20 + 60, widths, 20, "Evaluate");
	Fl_Button* button_show = new Fl_Button(x - 170, y - 20, widths, 20, "Show");
	Fl_Button* button_assing = new Fl_Button(20, 120, widths, 20, "Assign");

	button_eval->type(FL_NORMAL_BUTTON);
	button_eval->callback(eval_expression_callback, link);
	button_assing->type(FL_NORMAL_BUTTON);
	button_assing->callback(assign_expression_callback, var_link);
	grid(20, input_expression, evaluator);

	link->history = History;
	link->evaluator = evaluator;
	link->output = out_put;
	link->expression = input_expression;
	link->postfix = post_out_put;
	var_link->evaluator = evaluator;
	var_link->output = asignation_output;
	var_link->history = History;
	var_link->expression = asignation_input;
	window->end();
	window->show(argc, argv);

	
	//int result = Catch::Session().run(argc, argv);
	return Fl::run();
}