/*****************************************************************************

        Upsampler2xFpuTpl.h
        Author: Laurent de Soras, 2005

Upsamples by a factor 2 the input signal, using FPU.

Template parameters:
	- NC: number of coefficients, > 0

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (hiir_Upsampler2xFpuTpl_HEADER_INCLUDED)
#define hiir_Upsampler2xFpuTpl_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "pandaresampler/hiir/def.h"
#include "pandaresampler/hiir/StageDataFpu.h"

#include <array>



namespace PandaResampler
{

namespace hiir
{



template <int NC, typename DT>
class Upsampler2xFpuTpl
{

	static_assert ((NC > 0), "Number of coefficient must be positive.");

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef DT DataType;
	static constexpr int _nbr_chn  = 1;
	static constexpr int NBR_COEFS = NC;

	void           set_coefs (const double coef_arr [NBR_COEFS]);
	hiir_FORCEINLINE void
	               process_sample (DataType &out_0, DataType &out_1, DataType input);
	void           process_block (DataType out_ptr [], const DataType in_ptr [], long nbr_spl);
	void           clear_buffers ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	// Stages 0 and 1 contain only input memories
	typedef std::array <StageDataFpu <DataType>, NBR_COEFS + 2> Filter;

	Filter         _filter;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool           operator == (const Upsampler2xFpuTpl <NC, DT> &other) = delete;
	bool           operator != (const Upsampler2xFpuTpl <NC, DT> &other) = delete;

}; // class Upsampler2xFpuTpl



}  // namespace hiir

} // namespace PandaResampler



#include "pandaresampler/hiir/Upsampler2xFpuTpl.hpp"



#endif   // hiir_Upsampler2xFpuTpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
