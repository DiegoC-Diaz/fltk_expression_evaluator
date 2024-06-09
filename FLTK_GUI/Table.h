#pragma once
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Table.H>
#include "List.h"
void button_cb(Fl_Widget* w, void*);


class WidgetTable : public Fl_Table {
protected:
    void draw_cell(TableContext context,  		// table cell drawing
     int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
    

    LinkedList<Fl_Input*> values{};

public:
 
    WidgetTable(int x, int y, int w, int h, const char* l = 0) : Fl_Table(x, y, w, h, l) {
        col_header(1);
        col_resize(1);
        col_header_height(25);
        row_header(0);
        row_resize(1);
        row_header_width(80);
        end();
    }

    


    void show_contents() {
        Node<Fl_Input*>* current = values.getHead();
        while (current != nullptr) {
            std::cout << "Token: " << current->token->value() << " -Value: " << current->value->value()<<"\n";
            current = current->next;

        }

    }

    ~WidgetTable() { }
    void SetSize(int newrows, int newcols) {
        clear();		// clear any previous widgets, if any
        rows(newrows);
        cols(newcols);

        begin();		// start adding widgets to group
        {
            for (int r = 0; r < newrows; r++) {

                for (int c = 0; c < newcols; c++) {
                    int X, Y, W, H;
                    find_cell(CONTEXT_TABLE, r, c, X, Y, W, H);

                   
                    Fl_Input* in = new Fl_Input(X, Y, W, H);
                    in->value("");
                    if (c == 0) {
                        values.add(in);
                    }
                    else {
                        values.setValue(in, r);
                    }
                 

              
        
                }

               

            }
        }
        end();
    }

};



void WidgetTable::draw_cell(TableContext context,
    int R, int C, int X, int Y, int W, int H) {
    switch (context) {
    case CONTEXT_STARTPAGE:
        fl_font(FL_HELVETICA, 12);		// font used by all headers
        break;

    case CONTEXT_RC_RESIZE: {
        int X, Y, W, H;
        int index = 0;
        for (int r = 0; r < rows(); r++) {
            for (int c = 0; c < cols(); c++) {
                if (index >= children()) break;
                find_cell(CONTEXT_TABLE, r, c, X, Y, W, H);
                child(index++)->resize(X, Y, W, H);
            }
        }
        init_sizes();			// tell group children resized
        return;
    }

    case CONTEXT_ROW_HEADER:
        fl_push_clip(X, Y, W, H);
        {
            static char s[40];
            snprintf(s, sizeof(s), "The answer is %d", R);
            fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
            fl_color(FL_BLACK);
            fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
        }
        fl_pop_clip();
        return;

    case CONTEXT_COL_HEADER:
        fl_push_clip(X, Y, W, H);
        {

            static char s[40];
            if (C == 0) {
                snprintf(s, sizeof(s), "Token", 0);
            }
            else {
                snprintf(s, sizeof(s), "Varaible", 0);
            }
        
            fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, col_header_color());
            fl_color(FL_BLACK);
            fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
        }
        fl_pop_clip();
        return;

    case CONTEXT_CELL:
        return;		// fltk handles drawing the widgets

    default:
        return;
    }
}

WidgetTable *  callTable(int x, int y) {
    WidgetTable* table = new WidgetTable(x, y, 170, 390 - y, "Variables");
    table->SetSize(10, 2);
    table->show_contents();
    table->show_contents();
    return table;
}