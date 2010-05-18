//****************************************************************************
//	kaz_templates.h
//	Kazan's replacement for std::vector and std::string
//	std::vector is slow, and it's hard to debug with it floating around
//****************************************************************************

/*
 * $Logfile: /kaz_templates.h $
 * $Revision: 1.21 $
 * $Date: 2008/01/14 11:55:13 $
 * $Author: bobboau $
 *
 * $Log: kaz_templates.h,v $
 * Revision 1.21  2008/01/14 11:55:13  bobboau
 * syncroniseing with CVS, nothing important, invisible texture don't draw, and a few added utility functions for the string
 *
 * Revision 1.20  2008/01/10 18:28:10  bobboau
 * *** empty log message ***
 *
 * Revision 1.19  2007/07/15 17:35:16  bobboau
 * fixed STUPIDEST BUG EVER
 *
 * Revision 1.18  2007/07/10 20:48:24  kazan
 * Implement PMF -> COB
 *
 * Revision 1.17  2007/07/01 20:16:21  bobboau
 * commiting experimental multi-poly-per-node code
 *
 * Revision 1.16  2007/06/30 18:58:10  bobboau
 * made some tweaks to the auto-path feature, and added an orthographic projection option
 *
 * Revision 1.15  2007/06/29 20:15:43  bobboau
 * added copy and delete functionality for subobjects, there are situations were some higherarchies might break this, we should realy consiter holding the subobject data in an actual tree.
 *
 * also added a set/get active chunk function to the gl canvas, and implemented a wireframe overlay for the currently selected subobject. (that only renders when you have a subobject selected in the tree/editor)
 *
 * Revision 1.14  2007/06/25 19:39:33  kazan
 * Fix bug related to kaz_string and operator+= w/ char (not char*)
 * Fix autoturret bugs
 * Fix POF BSP compilation... finally.. confirmed models work in FS2!
 *
 * Revision 1.13  2007/06/25 13:13:15  kazan
 * had to remove bob's path prefix code from pcs2.ini load - was breaking it
 * moved ilutRenderer(ILUT_OPENGL) to fix 256x256 texture bug (#11)
 * added texture serach logging for debug builds
 *
 * Revision 1.12  2007/06/15 06:00:18  bobboau
 * number of small fixes, mostly related to updateing data in one subsystem when it should be
 *
 * fixed weapon point bug were the editing of one type would result in the eraseing of the other
 *
 * added PINF editor
 *
 * added convergence calculator for weapon points
 *
 * quickened the scroll zoom and made it proportional
 *
 * Revision 1.11  2007/06/06 12:08:22  bobboau
 * polished the UI some, converted a bunch of int_ctrls to int_lists, mostly for selecting a subobject number, added array over/underflow protection/handeling to some stuff.
 *
 * Revision 1.10  2007/06/05 06:29:24  bobboau
 * initial Bobboau UI commit, everything should be minimuly functional but working includeing editors for all major chuncks and the tree navigation control.
 *
 * Revision 1.9  2007/03/25 03:17:46  kazan
 * Implemented COB importer/autogen features - i haven't implemented them
 * cleaned up a bunch of signed/unsigned bitching - more cleaning to come
 *
 * Revision 1.8  2005/10/07 14:08:06  kazan
 * linux compat syncs - i'm getting memory crashes in linux, hopefully it happens in windows too so it's easier to debug
 *
 * Revision 1.7  2005/09/19 21:47:12  kazan
 * Linux compat changes
 *
 * Revision 1.6  2005/08/31 15:29:55  kazan
 * small behavioral fix for the wildcard search
 *
 * Revision 1.5  2005/08/30 23:23:25  kazan
 * working dds.. /shock
 *
 * Revision 1.4  2005/08/29 01:25:06  kazan
 * Saves POFs succesfully -- completely new BSP Gen routine
 * Loads and Renders Textures
 *
 * Revision 1.3  2005/08/28 02:39:06  kazan
 * Textures load and render!
 *
 * Revision 1.2  2005/08/25 15:06:38  kazan
 * various bugfixes and tweaks
 *
 * Revision 1.1  2005/08/23 22:36:42  kazan
 * s/std::{vector:string}/kaz_{vector:string}
 * renderer motion works
 * location PMF::LoadFromPOF bottleneck
 *
 *
 *
 *
 */

#if !defined(_kaz_vector_h_)
#define _kaz_vector_h_
#define _USE_MATH_DEFINES

#include <cstring>
#include <ostream>
#include <sstream>
#include <wx/wx.h>

#define MIN(a, b) ((a<b)?a:b)

#define MAX(a, b) ((a>b)?a:b)

//****************************************************************************
// kaz_vector declaration
//****************************************************************************


template <class vec_type> class kaz_vector
{
	protected:
		vec_type *array;
		unsigned int sz;

	public:
		kaz_vector() : array(NULL), sz(0) {}
		explicit kaz_vector(unsigned int sz) : array(NULL), sz(0)  { resize(sz); }
		kaz_vector(unsigned int sz, vec_type &initvalue);
		kaz_vector(unsigned int s, vec_type*ar):array(NULL), sz(0){
			resize(s);
			for(unsigned int i = 0; i<sz; i++){
				array[i] = ar[i];
			}
		}
		kaz_vector(const kaz_vector<vec_type> &src) : array(NULL), sz(0)
		{
			this->operator=(src);
		}

		~kaz_vector() { delete[] array; array=NULL; }

		virtual void operator=(const kaz_vector<vec_type> &src);

		virtual vec_type& operator[](unsigned int idx);

		
		virtual const vec_type& operator[](unsigned int idx) const;

		virtual void resize(unsigned int nsz);

		virtual void insert(unsigned int idx, vec_type item);
		//place item at position idx, move everything at or beond idx back one
		virtual void push_back(vec_type item);
		virtual void remove(int idx);
		virtual unsigned int size() const { return sz; }

		virtual void set(const vec_type&val);
		//sets everything in the array to the passed value safely

		virtual int get_index(vec_type*const item);
		//returns the index of the item if it is in the array, -1 if it is not

};


//****************************************************************************
// kaz_vector implementation
//****************************************************************************
template <class vec_type>
kaz_vector<vec_type>::kaz_vector(unsigned int sz, vec_type &initvalue)
{
	array = NULL;
	sz = 0;
	resize(sz);
	for (int i = 0; i < sz; i++)
		array[i] = initvalue;
}
		

template <class vec_type>
vec_type& kaz_vector<vec_type>::operator[](unsigned int idx)
{
	if (idx >= sz){
		std::stringstream gah;
		gah << "*ERROR*:Array out of bounds: " << idx << " >= " << sz << "! \nThat's BAD!";
		wxMessageBox(/*"*ERROR*:Array out of bounds! \nThat's BAD!"*/ gah.str().c_str(), "Oh my GOD it's so HORABLE!!!");
		exit(1); // crash the app! -- also make this a break :P
	}

	return array[idx];
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

template <class vec_type>
const vec_type& kaz_vector<vec_type>::operator[](unsigned int idx) const
{
	if (idx >= sz){
		wxMessageBox("*ERROR*:Array(const) out of bounds! \nThat's BAD!", "Oh my GOD it's so HORABLE!!!");
		exit(1); // crash the app! -- also make this a break :P
	}

	return array[idx];
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-


template <class vec_type>
void kaz_vector<vec_type>::operator=(const kaz_vector<vec_type> &src)
{
	if(this==&src)return;//this is bad if it goes any further

	sz = 0;
	if (this->array)
		delete[] this->array;

	this->sz = src.sz;
	this->array = new vec_type[this->sz];


	//memcpy(this->array, src.array, sizeof(vec_type) * this->sz);
	vec_type *linc=array, *sinc=src.array;

	while (linc < (array+sz))
	{
		*linc = *sinc;
		linc++;
		sinc++;
	}
}

template <class vec_type>
bool operator==(const kaz_vector<vec_type> &ths, const kaz_vector<vec_type> &src){
	if(ths.size() == src.size())
	{
		for(unsigned int i =0; i< ths.size(); i++){
			//this way it's defined if the template type is defined
			if( !(ths[i] == src[i]) ){
				return false;
			}
		}
		return true;
	}
	return false;
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

template <class vec_type>
void kaz_vector<vec_type>::resize(unsigned int nsz)
{
	//special-case
	if (nsz == 0)
	{
		if (array)
			delete[] array;

		sz = 0;
		array = NULL;
		return;
	}

	if (sz == 0)
	{
		sz = nsz;
		array = new vec_type[nsz];
		//memset(array, 0, sizeof(vec_type) * nsz);
		return;
	}
	// make the new memory chunk
	vec_type *narr = new vec_type[nsz];
	//memset((char *)narr, 0, sizeof(vec_type) * nsz);

	// copy the old data
	unsigned int cp = (sz < nsz)?sz:nsz;
	
	// memcpy isn't safe - not everything may be memcpy-able
	// memcpy(narr, array, sizeof(vec_type) * cp);
	vec_type *ninc=narr, *oinc=array;

	while (ninc < (narr+cp))
	{
		*ninc = *oinc;
		ninc++;
		oinc++;
	}

	//delete the old array and replace
	delete[] array;
	array = narr;
	sz = nsz;
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
template <class vec_type>
void kaz_vector<vec_type>::insert(unsigned int idx, vec_type item){
	resize(sz+1);
	if(sz>1)
	for(unsigned int i = sz-1; i>idx; i--){
		array[i] = array[i-1];
	}
	array[idx] = item;
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

template <class vec_type>
void kaz_vector<vec_type>::push_back(vec_type item){
	resize(sz+1);
	(*this)[size()-1]=item;
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-


//sets everything in the array to the passed value safely
template <class vec_type>
void kaz_vector<vec_type>::set(const vec_type&val){
	for(unsigned int i = 0; i<sz; i++){
		array[i] = val;
	}
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
template <class vec_type>
int kaz_vector<vec_type>::get_index(vec_type*const item){
	if(item<array || item > array+sz)
		return -1;
	return item - array;
}
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
template <class vec_type>
void kaz_vector<vec_type>::remove(int idx){
	if(idx<0)return;

	for(unsigned int i = idx; i<size()-1; i++){
		(*this)[i]=(*this)[i+1];
	}
	resize(size()-1);
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

class kaz_string : public kaz_vector<char>
{
	protected:
		unsigned int len;
	public:
		kaz_string() : kaz_vector<char>(), len(0) {}

		
		kaz_string(int sz) : kaz_vector<char>(), len(0) { resize(sz); array[0] = '\0'; }

		kaz_string(const char *str) : kaz_vector<char>(), len(0) 
		{
			this->resize(strlen(str));
			strncpy(array, str, strlen(str));
		}
		
		kaz_string(const char *str, unsigned int s) : kaz_vector<char>(), len(0) 
		{
			this->resize(MIN(strlen(str), s));
			strncpy(array, str, size()-1);
		}
		
		kaz_string(const kaz_string& other) : kaz_vector<char>(), len(0) 
		{
			this->resize(other.len);
			//this->len = other.len;
			strncpy(array, other.array, other.len);
		}

		~kaz_string() { delete[] array; array=NULL; }

		unsigned int length() const { return len; } 
		virtual void resize(unsigned int nsz) 
			{ 
				unsigned int osz = sz;
				kaz_vector<char>::resize(nsz+1); 
				this->array[nsz] = '\0';
				len=nsz; 
			}


		const char* c_str() const { return array; }

		kaz_string str_to_lower() const;
		kaz_string str_to_upper() const;

		//operators
		bool operator!=(const char *arg) const
		{
			return strcmp(array, arg) != 0;
		}

		bool operator!=(kaz_string &other) const
		{
			return strcmp(array, other.array) != 0;
		}
		
		bool operator==(const char *arg) const
		{
			return strcmp(array, arg) == 0;
		}

		bool operator==(const kaz_string &other) const
		{
			return strcmp(array, other.array) == 0;
		}

		kaz_string operator+(char c) const
		{
			kaz_string nstring(this->len + 1);
			strcat(nstring.array, this->array);
			nstring.array[this->len] = c;
			return nstring;
		}

		kaz_string operator+(const char *arg) const
		{
			if (this->len == 0)
				return kaz_string(arg);
			kaz_string nstring(this->len + strlen(arg));
			strcat(nstring.array, this->array);
			strcat(nstring.array, arg);
			return nstring;
		}

		kaz_string operator+(const kaz_string &other) const
		{
			if (this->len == 0)
				return other;
			kaz_string nstring(this->len + other.len);
			strcat(nstring.array, this->array);
			strcat(nstring.array, other.array);
			return nstring;
		}

		void operator+=(const char arg)
		{
			*this = *this + arg;
			//this->resize(this->len + strlen(arg));
			//strcat(this->array, arg);
		}

		void operator+=(const char *arg)
		{
			*this = *this + arg;
			//this->resize(this->len + strlen(arg));
			//strcat(this->array, arg);
		}

		void operator+=(const kaz_string &other)
		{
			*this = *this + other;
			//this->resize(this->len + other.len);
			//strcat(this->array, other.array);
		}

		void operator=(const char *arg)
		{
			this->resize(strlen(arg));
			strncpy(this->array, arg, strlen(arg));
		}

		void operator=(const kaz_string &other)
		{
			this->resize(other.len);
			strncpy(this->array, other.array, other.len);
		}		

		kaz_string substr(unsigned int start, unsigned int length=0)
		{
			kaz_string strtmp;
			char *cstr;
			unsigned int size;

			if (start+length > sz)
				length = sz-start;

			if (length > 0)
			{
				size = length;
			}
			else
			{
				size = sz-start;
			}
			
			cstr = new char[size+1];
			memset(cstr, 0, size+1);
			memcpy(cstr, array+start, size);
			strtmp = cstr;
			delete[] cstr;
			return strtmp;
		}

		//repllace all a with b
		virtual void replace(const char&a, const char&b){
			for(unsigned int i = 0; i<sz-1; i++){
				if(array[i] == a)
					array[i] = b;
			}
		}

		virtual unsigned int find(const char&item){
			for(unsigned int i = 0; i< sz-1; i++){
				if(array[i] == item)return i;
			}
			return 0xffffffff;
		}
};



std::ostream& operator<<(std::ostream&, const kaz_string&);

#endif //_kaz_vector_h_

