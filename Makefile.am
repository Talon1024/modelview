AUTOMAKE_OPTIONS = foreign
EXTRA_DIST = autogen.sh \
	down.xpm \
	left.xpm \
	new.xpm \
	open.xpm \
	pcsico.xpm \
	placeholder.xpm \
	right.xpm \
	save.xpm \
	solid.xpm \
	textured.xpm \
	up.xpm \
	wireframe.xpm \
	xleft.xpm \
	xright.xpm \
	yleft.xpm \
	yright.xpm \
	zin.xpm \
	zleft.xpm \
	zout.xpm \
	zright.xpm


bin_PROGRAMS = pcs2

pcs2_LDADD = -lIL -lILU -lILUT $(top_builddir)/compat/libcompat.a $(WX_LIBS)
pcs2_SOURCES = BSPDataStructs.cpp \
	BSPHandler.cpp \
	COBHandler.cpp \
	FileList.cpp \
	kaz_templates.cpp \
	pcs2.cpp \
	pcs2_filethread.cpp \
	PCS2_MainPanel.cpp \
	pcs_file.cpp \
	pcs_file_dstructs.cpp \
	pcs_pmf_cob.cpp \
	pcs_pmf_pof.cpp \
	pcs_pof_bspfuncs.cpp \
	POFHandler.cpp \
	tex_ctrl.cpp \
	vector3d.cpp \
	VPReader.cpp \
	wxCTreeCtrl.cpp \
	wxGL_PMFCanvas.cpp \
	wxPCSPref.cpp \
	pe_docking.cpp \
	pe_glows.cpp \
	pe_header.cpp \
	pe_paths.cpp \
	pe_subobjects.cpp \
	pe_subsystems.cpp \
	pe_textures.cpp \
	pe_thrusters.cpp \
	pe_turrets.cpp \
	pe_weapons.cpp 

noinst_HEADERS = BSPDataStructs.h \
	BSPHandler.h \
	COBDataStructs.h \
	COBHandler.h \
	FileList.h \
	kaz_templates.h \
	pcs2_CIDs.h \
	pcs2_filethread.h \
	pcs2.h \
	pcs_file_dstructs.h \
	pcs_file.h \
	pcs_pof_bspfuncs.h \
	POFDataStructs.h \
	POFHandler.h \
	resource.h \
	tex_ctrl.h \
	vector3d.h \
	VPReader.h \
	wxCTreeCtrl.h \
	wxPCSPref.h \
	pe_base.h \
	pe_docking.h \
	pe_glows.h \
	pe_header.h \
	pe_paths.h \
	pe_subobjects.h \
	pe_subsystems.h \
	pe_textures.h \
	pe_thrusters.h \
	pe_turrets.h \
	pe_weapons.h 

SUBDIRS = compat
