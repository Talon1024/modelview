//****************************************************************************
//	wxPCSPref.cpp
//	Preferences Dialog for PCS2
//	
//****************************************************************************

/*
 * $Logfile: /wxPCSPref.cpp $
 * $Revision: 1.11 $
 * $Date: 2008/05/15 19:19:24 $
 * $Author: kazan $
 *
 * $Log: wxPCSPref.cpp,v $
 * Revision 1.11  2008/05/15 19:19:24  kazan
 * Resolve #35, #64
 * Maybe Resolve: #68, #61
 *
 * Revision 1.10  2007/07/13 11:31:06  bobboau
 * added auto-extraction and external loading of textures in VPs,  and a user defined temp directory.
 *
 * Revision 1.9  2007/07/06 05:38:56  bobboau
 * added a grid, fixed a few bugs, mostly related to othodanal view.
 *
 * Revision 1.8  2007/06/25 15:57:04  kazan
 * recommit fixed path stuff for the config
 *
 * Revision 1.7  2007/06/25 13:13:15  kazan
 * had to remove bob's path prefix code from pcs2.ini load - was breaking it
 * moved ilutRenderer(ILUT_OPENGL) to fix 256x256 texture bug (#11)
 * added texture serach logging for debug builds
 *
 * Revision 1.6  2007/06/25 04:14:10  bobboau
 * implemented file assosiation, for windows at least, also made a makeshift icon for the exe, as pofs get there icons changed to reflect assosiation.
 * also made sure that the ini file gets saved and loaded only to the same folder as the exe it'self.
 *
 * Revision 1.5  2007/06/25 01:42:40  bobboau
 * added a options dialog for selecting editor colors.
 *
 * Revision 1.4  2007/06/16 19:40:11  bobboau
 * some small bugfixes, and added light color options.
 *
 * Revision 1.3  2007/03/22 13:15:33  kazan
 * MSVC8 compatability checkin, requires wxWidgets 2.8
 *
 * Revision 1.2  2005/08/31 16:59:24  kazan
 * Split control IDs into their own header, disable some context menus, get ready to start putting editing controls in.
 *
 * Revision 1.1  2005/08/28 00:35:06  kazan
 * new files for the config dialog and toolbar
 *
 *
 *
 */

 #include "wxPCSPref.h"
#include "pcs2_CIDs.h"
#include "pcs2.h"
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>
#include <wx/wfstream.h>
#include <wx/colordlg.h>
#include "color_options_dlg.h"

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

PCS_Preferences::PCS_Preferences(wxWindow *parent)
	: wxDialog(parent, -1, "PCS Preferences", wxDefaultPosition, wxSize(450,300))
{
	// ----------------- Create Controls ------------------
	texture_paths = new wxListBox(this, PCS2_PREF_LBOX, wxPoint(5,15), 
				wxSize(430, 80), 0);
	tpath_txt = new wxStaticText(this, -1,"Texture Paths", wxPoint(5, 0));

	
	tadd = new wxButton(this, PCS2_PREF_TADD,	"Add",			wxPoint(5, 100), wxSize(50, 20));
	tdele = new wxButton(this, PCS2_PREF_TDELE, "Delete",		wxPoint(60, 100), wxSize(50, 20));
	tbrowse = new wxButton(this, PCS2_PREF_TBROWSE, "Browse",	wxPoint(115, 100), wxSize(50, 20));

	tpath = new wxTextCtrl(this, PCS2_PREF_TCTRL, "", wxPoint(5, 125), wxSize(430, 20));

	// ------
	cob_scale_text = new wxStaticText(this, -1,"COB Scaling Factor", wxPoint(5, 200));
	cob_scale = new wxTextCtrl(this, PCS2_PREF_CSFTXT, "", wxPoint(25, 215), wxSize(50, 20));
	
	temp_path_title = new wxStaticText(this, -1,"Temporary Dir", wxPoint(5, 150));
	temp_path = new wxTextCtrl(this, PCS2_PREF_TMPD_TXT, "", wxPoint(60, 165), wxSize(375, 20));
	tbrowse = new wxButton(this, PCS2_PREF_TMPD_BTN, "Browse",	wxPoint(5, 165), wxSize(50, 20));

	geo_filter = new wxCheckBox(this, PCS2_PREF_CKBX_GFILT, "Use Geometry Filter", wxPoint(130, 200));
	vbos = new wxCheckBox(this, PCS2_PREF_CKBX_VBO, "Use OpenGL VBOs (if able)", wxPoint(250, 200));;

	// old positions
	/*def_col = new wxButton(this, PCS2_PREF_DEF_COL_BTN, "Diffuse",	wxPoint(170, 225), wxSize(50, 20));
	amb_col = new wxButton(this, PCS2_PREF_AMB_COL_BTN, "Ambient",	wxPoint(220, 225), wxSize(50, 20));
	col_ops = new wxButton(this, PCS2_PREF_COL_OP_BTN, "Color Options",	wxPoint(170, 245), wxSize(100, 20));*/

	def_col = new wxButton(this, PCS2_PREF_DEF_COL_BTN, "Diffuse",	wxPoint(270, 225), wxSize(50, 20));
	amb_col = new wxButton(this, PCS2_PREF_AMB_COL_BTN, "Ambient",	wxPoint(320, 225), wxSize(50, 20));
	col_ops = new wxButton(this, PCS2_PREF_COL_OP_BTN, "Color Options",	wxPoint(270, 245), wxSize(100, 20));

	// Collada options
	helpers = new wxCheckBox(this, PCS2_PREF_CKBX_HELPERS, "Export helpers to DAE", wxPoint(5, 240));
	props_as_helpers = new wxCheckBox(this, PCS2_PREF_CKBX_PROPS_AS_HELPERS, "Export properties as helpers", wxPoint(5, 255));

	dae_flip_axes = new wxStaticText(this, -1,"Mirror DAE axes", wxPoint(180, 240));
	mirror_x = new wxCheckBox(this, PCS2_PREF_CKBX_DAE_MIRROR_X, "X", wxPoint(180, 255));
	mirror_y = new wxCheckBox(this, PCS2_PREF_CKBX_DAE_MIRROR_Y, "Y", wxPoint(210, 255));
	mirror_z = new wxCheckBox(this, PCS2_PREF_CKBX_DAE_MIRROR_Z, "Z", wxPoint(240, 255));

	//wxID_CANCEL
	//wxID_OK
	ok = new wxButton(this, wxID_OK, "Ok",	wxPoint(385, 225), wxSize(50, 20));
	cancel = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(385, 245), wxSize(50, 20));

	// -----------------  Load Values ------------------
	
	wxConfigBase *pConfig = wxConfigBase::Get();
	pConfig->SetPath(_T("/tpaths/"));
	int num_paths = pConfig->Read("numpaths", 0l);
	int itemp;
	char cstr[16];
	wxString *strs = new wxString[num_paths];

	for (int i = 0; i < num_paths; i++)
	{
		sprintf(cstr, "path%d", i);
		pConfig->Read(cstr, &strs[i]);
	}
	texture_paths->InsertItems(num_paths, strs, 0);
	delete[] strs;

	wxString temp;
	pConfig->Read("temp_path", &temp, get_temp_path());
	temp_path->ChangeValue(temp);

	pConfig->SetPath(_T("/convoptions/"));
	double d;
	pConfig->Read("cobscale", &d);
	
	sprintf(cstr, "%.2f", (float)d);
	cob_scale->ChangeValue(cstr);

	pConfig->Read("use_geometry_filter", &itemp, 1); // default to on
	if (itemp == 1)
		this->geo_filter->SetValue(true);
	else
		this->geo_filter->SetValue(false);

	//light colors
	long r,g,b;
	pConfig->SetPath(_T("/gr_options/"));

	pConfig->Read("use_vertex_buffer_objects", &itemp, 0); // default to off
	if (itemp == 1)
		this->vbos->SetValue(true);
	else
		this->vbos->SetValue(false);

	pConfig->SetPath(_T("/gr_options/ambient"));
	pConfig->Read("R", &r, 25);
	pConfig->Read("G", &g, 25);
	pConfig->Read("B", &b, 25);

	amb_col->SetBackgroundColour(wxColour(r,g,b));
	amb_col->SetForegroundColour(wxColour((r+128)%255,(g+128)%255,(b+128)%255));

	pConfig->SetPath(_T("/gr_options/diffuse"));
	pConfig->Read("R", &r, 230);
	pConfig->Read("G", &g, 230);
	pConfig->Read("B", &b, 230);

	def_col->SetBackgroundColour(wxColour(r,g,b));
	def_col->SetForegroundColour(wxColour((r+128)%255,(g+128)%255,(b+128)%255));


	// Collada options
	pConfig->SetPath(_T("/collada_options"));
	pConfig->Read("export_helpers", &itemp, 1); // default to on
	if (itemp == 1)
		this->helpers->SetValue(true);
	else
		this->helpers->SetValue(false);

	pConfig->Read("export_properties_as_helpers", &itemp, 0); // default to off
	if (itemp == 1)
		this->props_as_helpers->SetValue(true);
	else
		this->props_as_helpers->SetValue(false);
	pConfig->Read("mirror_x", &itemp, 0); // default to off
	if (itemp == 1)
		this->mirror_x->SetValue(true);
	else
		this->mirror_x->SetValue(false);
	pConfig->Read("mirror_y", &itemp, 0); // default to off
	if (itemp == 1)
		this->mirror_y->SetValue(true);
	else
		this->mirror_y->SetValue(false);
	pConfig->Read("mirror_z", &itemp, 0); // default to off
	if (itemp == 1)
		this->mirror_z->SetValue(true);
	else
		this->mirror_z->SetValue(false);



	// usage-time-data
	curEdit = -1;
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void PCS_Preferences::OnOK(wxCommandEvent &event)
{
	if (tpath->GetValue() != ""){
		//if there was something the user 
		bool added = false;
		for(unsigned int i = 0; i<texture_paths->GetCount(); i++){
			if(texture_paths->GetString(i) == tpath->GetValue()){
				//hasn't added yet, ask them about it
				added = true;
				break;
			}
		}
		if(!added && wxMessageBox(wxString::Format("Did You Want to Add '%s'?", tpath->GetValue()), "Unsaved Changes", wxYES_NO) == wxYES){
			texture_paths->Append(tpath->GetValue());
		}

	}

	int i;
	wxConfigBase *pConfig = wxConfigBase::Get();

	// -------- Save the Paths -------- 
	pConfig->SetPath(_T("/tpaths/"));

	
	// clear current info
	int num_paths = pConfig->Read("numpaths", 0l);
	char cstr[18];
	for (i = 0; i < num_paths; i++)
	{
		sprintf(cstr, "path%d", i);
		pConfig->DeleteEntry(cstr);
	}

	// set the new info
	pConfig->Write("numpaths", (int)texture_paths->GetCount());
	for (i = 0; i < (int)texture_paths->GetCount(); i++)
	{
		sprintf(cstr, "path%d", i);
		pConfig->Write(cstr, texture_paths->GetString(i));
	}
	pConfig->Flush();

	pConfig->Write("temp_path", temp_path->GetValue());

	wxColour col;
	col = amb_col->GetBackgroundColour();
	pConfig->SetPath(_T("/gr_options/ambient/"));
	pConfig->Write("R", (byte)col.Red());
	pConfig->Write("G", (byte)col.Green());
	pConfig->Write("B", (byte)col.Blue());
	col = def_col->GetBackgroundColour();
	pConfig->SetPath(_T("/gr_options/diffuse/"));
	pConfig->Write("R", (byte)col.Red());
	pConfig->Write("G", (byte)col.Green());
	pConfig->Write("B", (byte)col.Blue());

	
	pConfig->SetPath(_T("/gr_options/"));
	if (this->vbos->IsChecked())
		pConfig->Write("use_vertex_buffer_objects", 1);
	else
		pConfig->Write("use_vertex_buffer_objects", 0);


	// -------- Save the COB Scaling factor -------- 

	pConfig->SetPath(_T("/convoptions/"));
	if (cob_scale->GetValue() != "")
	{
		pConfig->Write("cobscale", cob_scale->GetValue());
	}

	if (this->geo_filter->IsChecked())
		pConfig->Write("use_geometry_filter", 1);
	else
		pConfig->Write("use_geometry_filter", 0);

	// -------- Save Collada options -------- 
	pConfig->SetPath(_T("/collada_options/"));
	if (this->helpers->IsChecked())
		pConfig->Write("export_helpers", 1);
	else
		pConfig->Write("export_helpers", 0);

	if (this->props_as_helpers->IsChecked())
		pConfig->Write("export_properties_as_helpers", 1);
	else
		pConfig->Write("export_properties_as_helpers", 0);

	if (this->mirror_x->IsChecked())
		pConfig->Write("mirror_x", 1);
	else
		pConfig->Write("mirror_x", 0);

	if (this->mirror_y->IsChecked())
		pConfig->Write("mirror_y", 1);
	else
		pConfig->Write("mirror_y", 0);

	if (this->mirror_z->IsChecked())
		pConfig->Write("mirror_z", 1);
	else
		pConfig->Write("mirror_z", 0);



	pConfig->Flush();
	// -------- Flush to Disk -------- 
	wxFileOutputStream cfg_out(get_root_path()+wxString("\\pcs2.ini"));
//	wxMessageBox(wxString::Format("saveing config to \"%s\"",get_root_path()+wxString("\\pcs2.ini"));
	((wxFileConfig*)pConfig)->Save(cfg_out);

	// -------- Close ourself out ----------

	if (this->IsModal())
		EndModal(wxID_OK);
	else
	{
		SetReturnCode(wxID_OK);
		Show(false);
	}
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void PCS_Preferences::onAdd(wxCommandEvent &event)
{
	if (tpath->GetValue() != "")
	{
		texture_paths->Append(tpath->GetValue());
		texture_paths->SetSelection(texture_paths->GetCount()-1);
	}
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void PCS_Preferences::onEdit(wxCommandEvent &event)
{
	if ((curEdit = texture_paths->GetSelection()) != wxNOT_FOUND && tpath->GetValue() != "")
	{
		texture_paths->SetString(curEdit, tpath->GetValue());
	}
}

void PCS_Preferences::onSelection(wxCommandEvent &event){
	if (curEdit != -1 && tpath->GetValue() != "")
	{
		texture_paths->SetString(curEdit, tpath->GetValue());
		tpath->ChangeValue("");
		curEdit = -1;
	}
	if (texture_paths->GetSelection() != wxNOT_FOUND)
	{
		curEdit = texture_paths->GetSelection();
		tpath->ChangeValue(texture_paths->GetString(curEdit));
	}
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void PCS_Preferences::onDelete(wxCommandEvent &event)
{
	if (texture_paths->GetSelection() != wxNOT_FOUND)
	{
		texture_paths->Delete(texture_paths->GetSelection());
		tpath->ChangeValue("");
	}
}


//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void PCS_Preferences::onBrowse(wxCommandEvent &event){
	wxString dir = wxDirSelector("Select New Texture Path", tpath->GetValue());
	if(!dir.empty()){
		texture_paths->Append(dir);
		texture_paths->SetSelection(texture_paths->GetCount()-1);
		tpath->ChangeValue(dir);
	}
}

void PCS_Preferences::on_color(wxCommandEvent &event){
	wxColour col = wxGetColourFromUser(this, 
		((wxButton*)(event.GetEventObject()))->GetBackgroundColour(), 
		((wxButton*)(event.GetEventObject()))->GetLabel());
	if(!col.IsOk())return;

	((wxButton*)(event.GetEventObject()))->SetBackgroundColour(col), 
	((wxButton*)(event.GetEventObject()))->SetForegroundColour(wxColour((col.Red()+128)%255,(col.Green()+128)%255,(col.Blue()+128)%255));

}

void PCS_Preferences::on_color_op(wxCommandEvent &event){
	color_options_dlg(this);
}

void PCS_Preferences::on_temp(wxCommandEvent &event){
	wxString dir = wxDirSelector("Select New Texture Path", tpath->GetValue());
	if(!dir.empty()){
		temp_path->ChangeValue(dir);
	}
}

BEGIN_EVENT_TABLE(PCS_Preferences, wxDialog)
	EVT_BUTTON(PCS2_PREF_TADD, PCS_Preferences::onAdd)
	EVT_LISTBOX(PCS2_PREF_LBOX, PCS_Preferences::onSelection)
	EVT_TEXT(PCS2_PREF_TCTRL, PCS_Preferences::onEdit)
	EVT_BUTTON(PCS2_PREF_TDELE, PCS_Preferences::onDelete)
	EVT_BUTTON(PCS2_PREF_TBROWSE, PCS_Preferences::onBrowse)
	EVT_BUTTON(wxID_OK, PCS_Preferences::OnOK)
	EVT_BUTTON(PCS2_PREF_DEF_COL_BTN, PCS_Preferences::on_color)
	EVT_BUTTON(PCS2_PREF_AMB_COL_BTN, PCS_Preferences::on_color)
	EVT_BUTTON(PCS2_PREF_COL_OP_BTN, PCS_Preferences::on_color_op)
	EVT_BUTTON(PCS2_PREF_TMPD_BTN, PCS_Preferences::on_temp)
END_EVENT_TABLE()
