#pragma once
#include "editor.h"
#include <string>
#include "combo.h"
#include "text_ctrl.h"
#include "pcs2_CIDs.h"
#include "suggest.xpm"


inline std::string get_string(text_ctrl*tc){
	return std::string(tc->GetValue().c_str());
}
inline void set_string(text_ctrl*tc, const wxString&s){
	tc->ChangeValue(s);
}
inline void set_string(text_ctrl*tc, const std::string&s){
	tc->ChangeValue(s.c_str());
}

inline std::string get_string(wxStaticText*st){
	return std::string(st->GetLabel().c_str());
}
inline void set_string(wxStaticText*st, const wxString&s){
	st->SetLabel(s);
}
inline void set_string(wxStaticText*st, const std::string&s){
	st->SetLabel(s.c_str());
}

/*
Primitive controls are a set of common simple controles that 
consist of one text box with a title and a border around them 
this set of classes allows for a standard look and common code 
path for these similar controls which can be of any type (for 
which a value can be easily determined from a string).
*/

//the base primitive control the vast majority of functionality 
//comes from this, derived controls are only required to implement
//a function that takes a string and extracts a value
template<typename type, class data_ctrl = text_ctrl>
class primitive_ctrl :
	public editor<type>
{
protected:
	data_ctrl*text_box;
public:
	
	//constructor builds the text box and adds it to editor's static box sizer
	primitive_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title, int orient = wxHORIZONTAL, int Flags=0)
	:editor(parent,x,y,w,h, orient, Title)
	{
		//make the text editor
		text_box = new data_ctrl(this,-1,"",wxPoint(6+x,15+y),wxSize(w-12,20),Flags);
		//add it to our controls
		add_control(text_box, 1,wxEXPAND ,1);
	};
public:
	virtual ~primitive_ctrl(void){};
	DECLARE_EVENT_TABLE();

	void do_lose_focus(wxFocusEvent&){
		wxCommandEvent event(EDIT_DONE, GetId());
		GetEventHandler()->ProcessEvent(event);
	}
//	void do_data_change(wxCommandEvent&){
//		wxCommandEvent event(EDIT_DONE, GetId());
//		GetEventHandler()->ProcessEvent(event);
//	}
};

BEGIN_EVENT_TABLE_TEMPLATE2 (primitive_ctrl,editor<type>,type,data_ctrl)
//	EVT_TEXT(wxID_ANY, primitive_ctrl::do_data_change)
	EVT_KILL_FOCUS(primitive_ctrl::do_lose_focus)
END_EVENT_TABLE()



//class for string interface
template<class data_ctrl>
class _string_ctrl
	:public primitive_ctrl<std::string, data_ctrl>
{

public:
	//the main accessors, gets/sets the value in the text box
	virtual std::string get_value(){return get_string(text_box);}
	virtual void set_value(const std::string&s){set_string(text_box, s);}

	//just passes parameters to primitive, and makes sure the box is clear, only doing that for completeness sake
	_string_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title)
		:primitive_ctrl(parent,x,y,w,h,Title)
	{
		set_value("");
	}
};
class string_ctrl : public _string_ctrl<text_ctrl>{public:string_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title):_string_ctrl(parent,x,y,w,h,Title){}};
class string_disp : public _string_ctrl<wxStaticText>{public:string_disp(wxWindow*parent, int x, int y, int w, int h, wxString Title):_string_ctrl(parent,x,y,w,h,Title){}};


//class for multi-line string interface
template<class data_ctrl = text_ctrl>
class _multi_string_ctrl
	:public primitive_ctrl<std::string, data_ctrl>
{

public:
	//the main accessors, gets/sets the value in the text box
	virtual std::string get_value(){return get_string(text_box);}
	virtual void set_value(const std::string&s){set_string(text_box, s);}

	//just passes parameters to primitive, and makes sure the box is clear, only doing that for completeness sake
	_multi_string_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title)
		:primitive_ctrl(parent,x,y,w,h,Title, wxHORIZONTAL,wxTE_MULTILINE)
	{
		set_value("");
	}
};
class multi_string_ctrl : public _multi_string_ctrl<text_ctrl>{public:multi_string_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title):_multi_string_ctrl(parent,x,y,w,h,Title){}};
class multi_string_disp : public _multi_string_ctrl<wxStaticText>{public:multi_string_disp(wxWindow*parent, int x, int y, int w, int h, wxString Title):_multi_string_ctrl(parent,x,y,w,h,Title){}};

//class for float interface
template<class data_ctrl = text_ctrl>
class _float_ctrl
	:public primitive_ctrl<float, data_ctrl>
{

public:
	//the main accessors, gets/sets the value in the text box
	virtual float get_value(){return (float)atof(get_string(text_box).c_str());}
	virtual void set_value(const float&f){set_string(text_box, wxString().Format("%f",f));}

	//just passes parameters to primitive, and makes sure the box is set to a proper default
	_float_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title)
		:primitive_ctrl(parent,x,y,w,h,Title)
	{
		set_value(0.0f);
	}
};
class float_ctrl : public _float_ctrl<text_ctrl>{public:float_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title):_float_ctrl(parent,x,y,w,h,Title){}};
class float_disp : public _float_ctrl<wxStaticText>{public:float_disp(wxWindow*parent, int x, int y, int w, int h, wxString Title):_float_ctrl(parent,x,y,w,h,Title){}};

//class for int interface
template<class data_ctrl = text_ctrl>
class _int_ctrl
	:public primitive_ctrl<int, data_ctrl>
{

public:
	//the main accessors, gets/sets the value in the text box
	virtual int get_value(){return (int)atoi(get_string(text_box).c_str());}
	virtual void set_value(const int&i){set_string(text_box, wxString().Format("%d",i));}

	//just passes parameters to primitive, and makes sure the box is set to a proper default
	_int_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title)
		:primitive_ctrl(parent,x,y,w,h,Title)
	{
		set_value(0);
	}
};
class int_ctrl : public _int_ctrl<text_ctrl>{public:int_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title):_int_ctrl(parent,x,y,w,h,Title){}};
class int_disp : public _int_ctrl<wxStaticText>{public:int_disp(wxWindow*parent, int x, int y, int w, int h, wxString Title):_int_ctrl(parent,x,y,w,h,Title){}};


//class for vector interface
template<class data_ctrl = text_ctrl>
class _vector_ctrl
	:public primitive_ctrl<vector3d, data_ctrl>
{

public:
	//the main accessors, gets/sets the value in the text box
	virtual vector3d get_value(){
		wxString s = get_string(text_box).c_str();
		if(s.Length()<1)return vector3d(0,0,0);

		float x = (float)atof(s.c_str());
		s.Remove(0,s.find_first_of(':'));
		if(s.Length()<1)return vector3d(x,0,0);
		s.Remove(0,1);

		float y = (float)atof(s.c_str());
		s.Remove(0,s.find_first_of(':'));
		if(s.Length()<1)return vector3d(x,y,0);
		s.Remove(0,1);

		float z = (float)atof(s.c_str());
		return vector3d(x,y,z);
	}
	virtual void set_value(const vector3d&v){set_string(text_box, wxString().Format("%f:%f:%f",v.x,v.y,v.z));}

	//just passes parameters to primitive, and makes sure the box is set to a proper default
	_vector_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title)
		:primitive_ctrl(parent,x,y,w,h,Title)
	{
		set_value(vector3d(0,0,0));
	}
};
class vector_ctrl : public _vector_ctrl<text_ctrl>{public:vector_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title):_vector_ctrl(parent,x,y,w,h,Title){}};
class vector_disp : public _vector_ctrl<wxStaticText>{public:vector_disp(wxWindow*parent, int x, int y, int w, int h, wxString Title):_vector_ctrl(parent,x,y,w,h,Title){}};

template<class data_ctrl = text_ctrl>
class _normal_ctrl
	:public _vector_ctrl<data_ctrl>
{
	wxButton*norm_btn;

public:
	_normal_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title)
		:_vector_ctrl<data_ctrl>(parent,x,y,w,h,Title)
	{
		add_control(norm_btn = new wxButton(this, NORM_NORMALIZE, "Norm", wxDefaultPosition, wxSize(40,20)));
		norm_btn->SetToolTip("Normalize\nMake Unit length");
	}

	void on_normalize(wxCommandEvent& event){
		set_value(MakeUnitVector(get_value()));
	}

};
class normal_ctrl : public _normal_ctrl<text_ctrl>{public:normal_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title):_normal_ctrl(parent,x,y,w,h,Title){};DECLARE_EVENT_TABLE();};
class normal_disp : public _normal_ctrl<wxStaticText>{public:normal_disp(wxWindow*parent, int x, int y, int w, int h, wxString Title):_normal_ctrl(parent,x,y,w,h,Title){}};



//class for matrix interface
//this might be too complex to be called a primitive control
class matrix_ctrl
	:public primitive_ctrl<matrix>
{
	text_ctrl*text_box2;
	text_ctrl*text_box3;

	vector3d get_vector(text_ctrl*txt_box){
		wxString s = txt_box->GetValue();
		if(s.Length()<1)return vector3d(0,0,0);

		float x = (float)atof(s.c_str());
		s.Remove(0,s.find_first_of(':'));
		if(s.Length()<1)return vector3d(x,0,0);
		s.Remove(0,1);

		float y = (float)atof(s.c_str());
		s.Remove(0,s.find_first_of(':'));
		if(s.Length()<1)return vector3d(x,y,0);
		s.Remove(0,1);

		float z = (float)atof(s.c_str());
		return vector3d(x,y,z);
	}
public:
	//the main accessors, gets/sets the value in the text box
	virtual matrix get_value(){
		return matrix(get_vector(text_box),get_vector(text_box2),get_vector(text_box3));
	}
	virtual void set_value(const matrix&m){
		text_box->ChangeValue(wxString().Format("%e:%e:%e",m.a2d[0][0],m.a2d[0][1],m.a2d[0][2]));
		text_box2->ChangeValue(wxString().Format("%e:%e:%e",m.a2d[1][0],m.a2d[1][1],m.a2d[1][2]));
		text_box3->ChangeValue(wxString().Format("%e:%e:%e",m.a2d[2][0],m.a2d[2][1],m.a2d[2][2]));
	}

	//just passes parameters to primitive, and makes sure the box is set to a proper default
	matrix_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title)
		:primitive_ctrl(parent,x,y,w,h,Title, wxVERTICAL)
	{
		//make the text editor
		text_box2 = new text_ctrl(this,-1,"",wxPoint(6+x,15+y),wxSize(w-12,20));
		text_box3 = new text_ctrl(this,-1,"",wxPoint(6+x,15+y),wxSize(w-12,20));
		//add it to our controls
		add_control(text_box2, 1,wxEXPAND ,1);
		add_control(text_box3, 1,wxEXPAND ,1);

		set_value(matrix());
	}
};

//has a button which when clicked will provide some common opptions
//ctrl is assumed to be a primitive_ctrl derived object
#define SUGGEST_REPLACE 0
#define SUGGEST_APPEND  1
template<typename type, class ctrl>
class suggest_ctrl
	:public ctrl
{
	wxBitmapButton*suggest_btn;

	std::vector<type>options;

	int mode;

public:
	suggest_ctrl(wxWindow*parent, int x, int y, int w, int h, wxString Title, std::vector<type>Options, int Mode = SUGGEST_REPLACE)
		:ctrl(parent,x,y,w,h,Title),options(Options), mode(Mode)
	{
		add_control(suggest_btn = new wxBitmapButton(this, STRING_SUGGEST, wxBitmap(suggest), wxDefaultPosition, wxSize(16,16)));
		suggest_btn->SetToolTip("Suggest\nProvide Common Options");
	}

	DECLARE_EVENT_TABLE();
	void on_suggest(wxCommandEvent& event){
		wxMenu pop("");
		for(unsigned int i = 0; i<options.size(); i++){
			if(options[i] == "")
				pop.Append(wxID_SEPARATOR);
			else
				pop.Append(i,options[i].c_str());
		}
		PopupMenu(&pop);
	}
	void on_suggest_menu(wxCommandEvent& event){
		switch(mode){
		case SUGGEST_APPEND:
			text_box->ChangeValue(text_box->GetValue() + 
				wxString(options[event.GetId()].c_str()));
			break;
		case SUGGEST_REPLACE:
		default:
			text_box->ChangeValue(options[event.GetId()].c_str());
			break;
		}
		text_box->SetFocus();
		text_box->SetInsertionPointEnd();
	}


};

BEGIN_EVENT_TABLE_TEMPLATE2 (suggest_ctrl, primitive_ctrl<type>, type, ctrl)
	EVT_BUTTON(STRING_SUGGEST,suggest_ctrl::on_suggest)
	EVT_MENU(wxID_ANY,suggest_ctrl::on_suggest_menu)
END_EVENT_TABLE()


//used to make primitive lists
template<typename type>
struct primitive_list_item{
	std::string title;
	type data;
};




//primitive list of options, rather than a direct editor it has a combo box
//with available options
template<typename type>
class primitive_list_ctrl :
	public editor<type>
{
protected:
	combo*combo_box;
	std::vector<type> option;

	std::vector<primitive_list_item<type>>cur_list;
public:
	void set_item(const int&i){combo_box->Select(i);}

	virtual type get_value(){
		int idx = combo_box->GetSelection();

		if(idx>-1 && idx<(int)combo_box->GetCount())
			return option[combo_box->GetSelection()];
		else if(option.size()>=1)
			return option[0];
		else 
			return type();//emergency failure
	}

	virtual void set_value(const type&i){
		for(unsigned int a =0; a<option.size(); a++){
			if(option[a] == i){
				combo_box->Select(a);
				return;
			}
		}
	}

	void build_list(const std::vector<primitive_list_item<type>>&list){
		combo_box->Clear();
		option.resize(list.size());
		for(unsigned int i = 0; i<list.size(); i++){
			combo_box->Append(list[i].title.c_str());
			option[i]=list[i].data;
		}
		if(list.size())
			combo_box->Select(0);

		cur_list=list;
	}
	
	//constructor builds the text box and adds it to editor's static box sizer
	primitive_list_ctrl(wxWindow*parent, const std::vector<primitive_list_item<type>>&list, int x, int y, int w, int h, wxString Title, int orient = wxHORIZONTAL, int Flags=0)
	:editor(parent,x,y,w,h, orient, Title)
	{
		//make the text editor
		combo_box = new combo(this,-1,"",wxPoint(6+x,15+y),wxSize(w-12,20),Flags);
		//add it to our controls
		add_control(combo_box, 1,wxEXPAND ,1);

		build_list(list);
	};

	virtual ~primitive_list_ctrl(void){};

	//by default don't change anything
	virtual std::vector<primitive_list_item<type>>&get_list(){
		return cur_list;
	}


	DECLARE_EVENT_TABLE();
	void on_select(wxCommandEvent& event){
		int idx = combo_box->GetSelection();
		build_list(get_list());
		if(idx<(int)combo_box->GetCount())combo_box->Select(idx);
		else combo_box->Select(0);

		GetEventHandler()->ProcessEvent(wxCommandEvent(DATA_SELECTION_CHANGED, GetId()));
	}
};

BEGIN_EVENT_TABLE_TEMPLATE1 (primitive_list_ctrl,editor<type>,type)
//	EVT_COMBOBOX(wxID_ANY, primitive_list_ctrl::on_select)
	EVT_COMMAND(wxID_ANY, COMBO_CLICKED, primitive_list_ctrl::on_select)
END_EVENT_TABLE()




inline void make_int_list(const std::vector<std::string>&l, std::vector<primitive_list_item<int>>&ret){
	ret.resize(l.size());

	for(unsigned int i = 0; i< l.size(); i++){
		ret[i].title = l[i];
		ret[i].data = i;
	}
}

void get_model_list(std::vector<std::string>&);
void get_path_list(std::vector<std::string>&);
//had just about enough of include hell

class model_list_ctrl
	:public primitive_list_ctrl<int>
{
public:
	model_list_ctrl(wxWindow*Parent, int x, int y, int w, int h, wxString Title, int orient = wxHORIZONTAL, int Flags=0)
		:primitive_list_ctrl(Parent, get_list(), x, y, w, h, Title, orient, Flags)
	{
	}

	std::vector<primitive_list_item<int>>&get_list(){
		static std::vector<std::string> str;
		static std::vector<primitive_list_item<int>> l;
		get_model_list(str);
		make_int_list(str, l);
		return l;
	}
};

class path_list_ctrl
	:public primitive_list_ctrl<int>
{
public:
	path_list_ctrl(wxWindow*Parent, int x, int y, int w, int h, wxString Title, int orient = wxHORIZONTAL, int Flags=0)
		:primitive_list_ctrl(Parent, get_list(), x, y, w, h, Title, orient, Flags)
	{
	}

	std::vector<primitive_list_item<int>>&get_list(){
		static std::vector<std::string> str;
		static std::vector<primitive_list_item<int>> l;
		get_path_list(str);
		make_int_list(str, l);
		return l;
	}
};