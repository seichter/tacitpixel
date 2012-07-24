/*
 * Copyright (C) 1999-2012 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _TP_ARGUMENTS_H_
#define _TP_ARGUMENTS_H_ 1


#include <tp/globals.h>
#include <tp/array.h>
#include <tp/string.h>


/**
  * Argument parser for handling command line arguments
  */
class TP_API tpArguments {
public:

	/**
	  * Default constructor for assigning the arguments
	  * based on what has been passed in by a standard main.
	  *
	  * @param argc pointer to the argument count
	  * @param argv pointer to the argument values
	  */
	explicit tpArguments(int *argc, char* argv[]);


	bool get(const tpString &param) const;
	bool get(const tpString &param, tpString &value) const;
	bool get(const tpString &param, tpUInt &value) const;

	/**
	  * A reference to the arguments passed in
	  *
	  * @return a reference to the string array
	  */
	const tpStringArray& get() const { return mArguments; }

	/**
	  * Convenience method for Windows. Here we only get the
	  * whole string and need to tokenize it.
	  *
	  * @param args argument string
	  */
	void setArguments(const tpString& args);


	/**
	  * Convenience method.
	  *
	  * @param argc pointer to the argument count
	  * @param argv pointer to the argument values
	  */
	void setArguments(int *argc, char* argv[]);

protected:

	tpStringArray mArguments;

};

#endif
