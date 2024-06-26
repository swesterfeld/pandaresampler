/*****************************************************************************

        Upsampler2xFpuTpl.hpp
        Author: Laurent de Soras, 2005

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if defined (hiir_Upsampler2xFpuTpl_CURRENT_CODEHEADER)
	#error Recursive inclusion of Upsampler2xFpuTpl code header.
#endif
#define	hiir_Upsampler2xFpuTpl_CURRENT_CODEHEADER

#if ! defined (hiir_Upsampler2xFpuTpl_CODEHEADER_INCLUDED)
#define	hiir_Upsampler2xFpuTpl_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "pandaresampler/hiir/StageProcFpu.h"

#include <cassert>


namespace PandaResampler
{

namespace hiir
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: set_coefs
Description:
	Sets filter coefficients. Generate them with the PolyphaseIir2Designer
	class.
	Call this function before doing any processing.
Input parameters:
	- coef_arr: Array of coefficients. There should be as many coefficients as
		mentioned in the class template parameter.
Throws: Nothing
==============================================================================
*/

template <int NC, typename DT>
void	Upsampler2xFpuTpl <NC, DT>::set_coefs (const double coef_arr [NBR_COEFS])
{
	assert (coef_arr != nullptr);

	for (int i = 0; i < NBR_COEFS; ++i)
	{
		_filter [i + 2]._coef = DataType (coef_arr [i]);
	}
}



/*
==============================================================================
Name: process_sample
Description:
	Upsamples (x2) the input sample, generating two output samples.
Input parameters:
	- input: The input sample.
Output parameters:
	- out_0: First output sample.
	- out_1: Second output sample.
Throws: Nothing
==============================================================================
*/

template <int NC, typename DT>
void	Upsampler2xFpuTpl <NC, DT>::process_sample (DataType &out_0, DataType &out_1, DataType input)
{
	DataType       even = input;
	DataType       odd  = input;
	StageProcFpu <NBR_COEFS, DataType>::process_sample_pos (
		NBR_COEFS,
		even,
		odd,
		_filter.data ()
	);
	out_0 = even;
	out_1 = odd;
}



/*
==============================================================================
Name: process_block
Description:
	Upsamples (x2) the input sample block.
	Input and output blocks may overlap, see assert() for details.
Input parameters:
	- in_ptr: Input array, containing nbr_spl samples.
	- nbr_spl: Number of input samples to process, > 0
Output parameters:
	- out_0_ptr: Output sample array, capacity: nbr_spl * 2 samples.
Throws: Nothing
==============================================================================
*/

template <int NC, typename DT>
void	Upsampler2xFpuTpl <NC, DT>::process_block (DataType out_ptr [], const DataType in_ptr [], long nbr_spl)
{
	assert (out_ptr != nullptr);
	assert (in_ptr  != nullptr);
	assert (out_ptr >= in_ptr + nbr_spl || in_ptr >= out_ptr + nbr_spl);
	assert (nbr_spl > 0);

	long           pos = 0;
	do
	{
		process_sample (
			out_ptr [pos * 2    ],
			out_ptr [pos * 2 + 1],
			in_ptr [pos]
		);
		++ pos;
	}
	while (pos < nbr_spl);
}



/*
==============================================================================
Name: clear_buffers
Description:
	Clears filter memory, as if it processed silence since an infinite amount
	of time.
Throws: Nothing
==============================================================================
*/

template <int NC, typename DT>
void	Upsampler2xFpuTpl <NC, DT>::clear_buffers ()
{
	for (int i = 0; i < NBR_COEFS + 2; ++i)
	{
		_filter [i]._mem = 0;
	}
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace hiir

}  // namespace PandaResampler



#endif   // hiir_Upsampler2xFpuTpl_CODEHEADER_INCLUDED

#undef hiir_Upsampler2xFpuTpl_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
