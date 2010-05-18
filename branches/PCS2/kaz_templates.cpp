//****************************************************************************
//	kaz_templates.cpp
//	Kazan's replacement for std::vector and std::string
//	std::vector is slow, and it's hard to debug with it floating around
//****************************************************************************

/*
 * $Logfile: /kaz_templates.cpp $
 * $Revision: 1.5 $
 * $Date: 2007/03/25 03:17:46 $
 * $Author: kazan $
 *
 * $Log: kaz_templates.cpp,v $
 * Revision 1.5  2007/03/25 03:17:46  kazan
 * Implemented COB importer/autogen features - i haven't implemented them
 * cleaned up a bunch of signed/unsigned bitching - more cleaning to come
 *
 * Revision 1.4  2005/09/19 21:47:12  kazan
 * Linux compat changes
 *
 * Revision 1.3  2005/08/31 15:29:55  kazan
 * small behavioral fix for the wildcard search
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

#include "kaz_templates.h"

std::ostream& operator<<(std::ostream& os, const kaz_string& str)
{
	os << str.c_str();
	return os;
}

kaz_string kaz_string::str_to_lower() const
{
	kaz_string nstring(*this);

	for (unsigned int i = 0; i < len; i++)
	{
		if (nstring[i] >=  65&& nstring[i] <= 90)
			nstring[i] += 32;
	}
	return nstring;
}


kaz_string kaz_string::str_to_upper() const
{
	kaz_string nstring(*this);

	for (unsigned int i = 0; i < len; i++)
	{
		if (nstring[i] >=  97 && nstring[i] <= 122)
			nstring[i] -= 32;
	}
	return nstring;
}
