/*******************************************************************************
 *
 * Copyright (C) 2014-2018 Wave Computing, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/* snarfed from musl 0.9.11 */

#include <locale.h>
#include "low/_time_data.h"

static const char* c_time[] ={
	"Sun","Mon","Tue","Wed","Thu","Fri","Sat",
	"Sunday","Monday","Tuesday","Wednesday",
	"Thursday","Friday","Saturday",
	"Jan","Feb","Mar","Apr","May","Jun",
	"Jul","Aug","Sep","Oct","Nov","Dec",
	"January","February","March","April",
	"May","June","July","August",
	"September","October","November","December",
	"AM","PM",
	"%a %b %e %T %Y",
	"%m/%d/%y",
	"%H:%M:%S",
	"%I:%M:%S %p",
	"",
	"%m/%d/%y",
	"0123456789",
	"%a %b %e %T %Y",
	"%H:%M:%S" };

static const char c_messages[] = "^[yY]\0" "^[nN]";
static const char c_numeric[] = ".\0" "";

const char *__langinfo(nl_item item)
{
	int cat = item >> 16;
	int idx = item & 65535;
	const char *str;

	if (item == CODESET) return "UTF-8";
	
	switch (cat) {
	case 1:
		if (idx > 1) return NULL;
		str = c_numeric;
		break;
	case 2:
		if (idx > 0x31) return NULL;
		return (char*)c_time[idx];
	case 4:
		if (idx > 0) return NULL;
		str = "";
		break;
	case 5:
		if (idx > 1) return NULL;
		str = c_messages;
		break;
	default:
		return NULL;
	}

	for (; idx; idx--, str++) for (; *str; str++);
	return (char *)str;
}

