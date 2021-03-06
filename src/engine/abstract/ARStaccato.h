#ifndef ARStaccato_H
#define ARStaccato_H

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

#include "ARArticulation.h"

/** \brief Staccato Tag
*/
class ARStaccato : public ARArticulation
{		
public:			
	enum		tTypeStacc {REGULAR, HEAVY};

	virtual bool MatchEndTag(const char * s);

			 ARStaccato() { type = REGULAR; }
	virtual ~ARStaccato() {};

	virtual void setTagParameterList(TagParameterList& tpl);

	virtual void browse(TimeUnwrap& mapper) const;

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;
	
	tTypeStacc getType() const {return type;};

protected:
	static ListOfTPLs ltpls;
	tTypeStacc	type;
};

#endif
