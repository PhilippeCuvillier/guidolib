/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <sstream>

#include "ARAlter.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"
#include "secureio.h"

using namespace std;

ListOfTPLs ARAlter::ltpls(1);

ARAlter::ARAlter(ARAlter * p_savealter, ARAlter * copyalter ) 
	: ARMTParameter(-1,copyalter) 
{
	relativeTimePosition = TYPE_TIMEPOSITION(-1,1);
	setDuration (DURATION_0);
	rangesetting = RANGEDC;	// RANGEDC replaced by ONLY while impl. finalzed [DF 2001-04-20];
	savealter = p_savealter;
	alterstring = NULL;
	altervalue = NULL;

	if (copyalter)
	{
		if (copyalter->getTPAlterString())
			alterstring = TagParameterString::cast(copyalter->getTPAlterString()->getCopy());
		if (copyalter->getTPAlterValue())
			altervalue = TagParameterFloat::cast(copyalter->getTPAlterValue()->getCopy());
	}
	else altervalue = new TagParameterFloat(0);
}

ARAlter::~ARAlter()
{ 
	delete altervalue;
	delete alterstring;
}

void ARAlter::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(( "F,detune,0.0,r;S,text,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			delete altervalue;
			altervalue = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(altervalue);

			delete alterstring;
			alterstring = TagParameterString::cast(rtpl->RemoveHead());
			assert(alterstring);
	
			if (alterstring->TagIsNotSet()) {
				// then we can set it to the value ...
				stringstream sstr;
				sstr << altervalue->getValue() << ".2f";
				alterstring->setValue(sstr.str().c_str());
			}
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

std::string ARAlter::getAlterText() const
{ 
	return alterstring ? alterstring->getValue() : "";
}

float ARAlter::getDetune() const
{ 
	return altervalue ? altervalue->getValue() : 0;
}

void ARAlter::printName(std::ostream& os) const
{
    os << "ARAlter";
}

void ARAlter::printGMNName(std::ostream& os) const
{
    os << "\\alter";
}

void ARAlter::printParameters(std::ostream& os) const
{
    if (altervalue)
        os << "detune: " << altervalue->getValue() << "; ";

    ARMusicalTag::printParameters(os);
}
