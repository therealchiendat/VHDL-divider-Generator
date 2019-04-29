/*
Introduction
VHDL language generator
Author: Chien Dat Nguyen Dinh
Last Upate : 8 / 30 / 2017 1 : 06AM
Description : The code will initially ask for number of bits input.This
function will then generate a VHDL file and save it under
vhdlgen.vhd.
Note : The function only generate VHDL code, the pin planner is required
*/


#include <fstream>
#include <iostream>
#include <time.h>
#include <math.h> 
#include <string>.
using namespace std;

int main()
{
	// Variable identifier
	int bits, inbits, outbits, invcount, xorjump = 0, muxjump = 0;
	int numofxor, numofmux;
	double numofsel;
	time_t now = time(0);
	string fileID,filename;
	char currenttime[26];

	// initialize time of VHDL generate
	ctime_s(currenttime, sizeof currenttime, &now);

	// Prompt
	cout << "Enter number of bits: ";
	cin >> bits;
	//Promt calculation
	inbits = bits - 1;
	outbits = bits * 2 - 2;
	numofsel = ceil(log(bits));
	numofxor = (bits*(bits + 1)) / 2 - 2;
	numofmux = (bits - 1)*(bits) / 2 - 1;
	string presetval(outbits + 1, '0');
	// SOMETHING SOMETHING

	// Initialize text file
	fileID = "vhdlgen";
	filename = "vhdlgen.vhd";
	ofstream vhdlgen(filename.c_str()); //Creates an instance of ofstream, and opens vhdlgen.vhd

	cout << "Successfully open the document. \nWriting data...\n";
	// Write data into vhdl document.
	//Title
	vhdlgen << "-- Author: Chien Dat Nguyen Dinh \n"
		<< "-- Title : " << bits << " by " << bits << " Divider using VHDL \n"
		<< "-- Time  :" << currenttime
		//Library
		<< "\nLIBRARY ieee;\nUSE ieee.std_logic_1164.all;\n\n"
		//IO
		<< "\nENTITY " << fileID << " IS\n  PORT\n	(	\n"
		<< "	f	: IN  std_logic_vector(" << inbits << " downto 0); \n"
		<< "	g	: IN  std_logic_vector(" << inbits << " downto 0); \n"
		<< "	r	: OUT std_logic_vector(" << outbits << " downto 0)\n);\n"
		<< "END " << fileID << ";\n"
		//MAIN	
		<< "ARCHITECTURE behavioural of " << fileID << " is \n"
		<< "	--Initialize variable \n"
		<< "	SIGNAL s_g    : std_logic_vector(" << inbits << " downto 0);\n"
		<< "	SIGNAL s_xor  : std_logic_vector(" << numofxor << " downto 0);\n"
		<< "	SIGNAL s_mux  : std_logic_vector(" << numofmux << " downto 0);\n"
		<< "	SIGNAL s_sel  : std_logic_vector(" << numofsel - 1 << " downto 0);\n"
		<< "	SIGNAL s_mout : std_logic_vector(" << inbits << " downto 0);\n"
		<< "	SIGNAL s_r	  : std_logic_vector(" << outbits << " downto 0):= \"" << presetval << "\";\n"
		<< "	--Initialize function \n"
		<< "	FUNCTION f_priority_decoder(r_sel : IN std_logic_vector(" << inbits << " downto 0)) \n"
		<< "		return std_logic_vector is \n"
		<< "		variable v_code : std_logic_vector(" << numofsel - 1 << " downto 0);\n"
		<< "	BEGIN\n";
	for (int count = 0; count < pow(2, numofsel); count++)
	{
		invcount = pow(2, numofsel) - 1 - count;
		if (count == 0)
			vhdlgen << "		if	r_sel(" << invcount << ") ='1' then\n			v_code := \"";
		else
			vhdlgen << "		elsif	r_sel(" << invcount << ") ='1' then\n			v_code := \"";
		for (int offset = numofsel - 1; offset >= 0; offset--)
		{
			vhdlgen << ((count & (1 << offset)) >> offset);
		}
		vhdlgen << "\"; \n"
			<< std::endl;
	}
	vhdlgen << "		end if; \n		return std_logic_vector(v_code); \n	END FUNCTION f_priority_decoder; \n -- \n ";
	// Barel shifter 1		
	vhdlgen << "	FUNCTION f_barel_shifter1(\n		r_input : IN std_logic_vector(" << inbits << " downto 0);\n"
		<< "		r_sel   : IN std_logic_vector(" << numofsel - 1 << " downto 0))\n"
		<< "		return std_logic_vector	is \n"
		<< "		variable v_output : std_logic_vector(" << inbits << " downto 0);\n"
		<< "	BEGIN\n"
		<< "		case r_sel is \n";
	for (int count = 0; count < pow(2, numofsel); count++)
	{
		invcount = pow(2, numofsel) - 1 - count;
		vhdlgen << "		when \"";
		for (int offset = numofsel - 1; offset >= 0; offset--)
		{
			vhdlgen << ((count & (1 << offset)) >> offset);
		}
		vhdlgen << "\" => \n";
		for (int outcount = 0; outcount <= inbits; outcount++)
		{
			if (count <= outcount)
				vhdlgen << "		v_output(" << outcount << ") := r_input(" << outcount - count << "); \n ";
			else
				vhdlgen << "		v_output(" << outcount << ") := '0'; \n";
		}
		vhdlgen << std::endl;
	}
	vhdlgen << "		end case; \n"
		<< "	return std_logic_vector(v_output); \n"
		<< "	END FUNCTION f_barel_shifter1; \n \n -- \n \n ";

	// Shifter 2
	vhdlgen << "	FUNCTION f_barel_shifter2(\n"
		<< "	r_input : IN std_logic_vector(" << outbits << " downto 0); \n"
		<< "	r_sel   : IN std_logic_vector(" << numofsel - 1 << " downto 0)) \n"
		<< "	return std_logic_vector is \n"
		<< "	variable v_output : std_logic_vector(" << outbits << " downto 0); \n"
		<< "	BEGIN \n"
		<< "		case r_sel is \n";
	for (int count = 0; count < pow(2, numofsel); count++)
	{
		invcount = pow(2, numofsel) - 1 - count;
		vhdlgen << "		when \"";
		for (int offset = numofsel - 1; offset >= 0; offset--)
		{
			vhdlgen << ((count & (1 << offset)) >> offset);
		}
		vhdlgen << "\" => \n";
		for (int outcount = 0; outcount <= outbits; outcount++)
		{
			if (count > 0)
				if (outbits >= outcount + inbits + 1 + count)
					vhdlgen << "		v_output(" << outcount << ") := r_input(" << outcount + inbits << "); \n ";
				else if (inbits - outcount < count + 1 && inbits - outcount >0)
					vhdlgen << "		v_output(" << outcount << ") := '0'; \n";
				else if (outbits >= outcount + inbits - count)
					vhdlgen << "		v_output(" << outcount << ") := r_input(" << outcount + inbits - count << "); \n ";
				else
					vhdlgen << "		v_output(" << outcount << ") := '0'; \n";
			else
			{
				if (outbits >= outcount + inbits)
					vhdlgen << "		v_output(" << outcount << ") := r_input(" << outcount + inbits << "); \n ";
				else
					vhdlgen << "		v_output(" << outcount << ") := '0'; \n";
			}
		}
		vhdlgen << std::endl;
	}
	vhdlgen << "		end case; \n"
		<< "	return std_logic_vector(v_output); \n"
		<< "	END FUNCTION f_barel_shifter2; \n \n -- \n \n ";

	// Shifter 2
	vhdlgen << "FUNCTION f_demux(\n r_sel : IN std_logic_vector(" << numofsel - 1 << " downto 0)) \n"
		<< "	return std_logic_vector is \n"
		<< "	variable v_output : std_logic_vector(" << inbits << " downto 0); \n"
		<< "	BEGIN \n"
		<< "		case r_sel is \n";
	for (int count = 0; count < pow(2, numofsel); count++)
	{
		invcount = pow(2, numofsel) - 1 - count;
		vhdlgen << "		when \"";
		for (int offset = numofsel - 1; offset >= 0; offset--)
		{
			vhdlgen << ((count & (1 << offset)) >> offset);
		}
		vhdlgen << "\" =>";

		if (count <= 1)
			vhdlgen << "		v_output(" << inbits - count << ") :='1'; \n ";
		else
			vhdlgen << "		v_output := (" << inbits - count << " to " << inbits - 1 << " => '1', others => '0'); \n";

		vhdlgen << std::endl;
	}
	vhdlgen << "	end case; \n"
		<< "	return std_logic_vector(v_output); \n"
		<< "	END FUNCTION f_demux; \n \n -- \n \n ";

	// Main 
	vhdlgen << "BEGIN \n	-- Compensate for g starts with zeros \n"
		<< "	s_sel    <= f_priority_decoder(g); \n"
		<< "	s_g      <= f_barel_shifter1(g,s_sel); \n"
		<< "	s_mout   <= NOT(f_demux(s_sel)); \n\n";

	vhdlgen << "	--Layer number 1 \n";
	for (int count = 0; count <= inbits; count++)
	{
		vhdlgen << "	s_xor(" << count << ") <= f(" << inbits - count << ") XOR s_g(" << inbits - count << "); \n";
	}
	for (int count = 0; count < inbits; count++)
	{
		if (count == 0)
			vhdlgen << "	s_mux(" << count << ") <= s_xor(" << count + 1 << ") when (s_xor(0)='0') else f(" << inbits - count - 1 << "); \n";
		else
			vhdlgen << "	s_mux(" << count << ") <= s_xor(" << count + 1 << ") when ((s_mout(" << count << ") AND s_xor(0))='0') else f(" << inbits - count - 1 << ");\n";
	}



	// PRETTY GOOD!!!
	for (int layer = 2; layer < bits; layer++)
	{
		xorjump = bits + 2 - layer + xorjump;
		muxjump = inbits + 2 - layer + muxjump;
		vhdlgen << "	--Layer number " << layer << "\n";
		for (int count = 0; count <= inbits - layer + 1; count++)
		{
			vhdlgen << "	s_xor(" << count + xorjump << ") <= s_mux(" << count + muxjump - inbits - 2 + layer << ") XOR s_g(" << inbits - count << "); \n";
		}
		for (int count = 0; count < inbits - layer + 1; count++)
		{
			if (count == 0)
				vhdlgen << "	s_mux(" << count + muxjump << ") <= s_xor(" << count + xorjump + 1 << ") when (s_xor(" << xorjump << ")='0') else s_mux(" << count - 1 + muxjump - (inbits - layer) << "); \n";
			else
				vhdlgen << "	s_mux(" << count + muxjump << ") <= s_xor(" << count + xorjump + 1 << ") when ((s_mout(" << count << ") AND s_xor(" << xorjump << "))='0') else s_mux(" << count - 1 + muxjump - (inbits - layer) << ");\n";
		}
	}


	// Result (NOT DONE!!!)
	vhdlgen << "\n	--Result \n"
		<< "	process (s_sel,s_mux,s_xor,f) \n"
		<< "	begin \n";
	muxjump = 0;
	for (int count = 0; count < pow(2, numofsel); count++)
	{
		invcount = pow(2, numofsel) - 1 - count;
		if (count == 0)
			vhdlgen << "		if		s_sel = \"";
		else
			vhdlgen << "		elsif	s_sel = \"";

		for (int offset = numofsel - 1; offset >= 0; offset--)
		{
			vhdlgen << ((count & (1 << offset)) >> offset);
		}
		vhdlgen << "\" then \n";
		xorjump = 0;
		if (count != 0)
			muxjump = inbits - count + muxjump;
		for (int outcount = 0; outcount < bits; outcount++)
		{

			if (count == inbits)
				vhdlgen << "			s_r(" << outbits - outcount << ")	<= f(" << inbits - outcount << "); \n";
			else if (outcount < count + 1)
				vhdlgen << "			s_r(" << outbits - outcount << ")	<= NOT(s_xor(" << xorjump << ")); \n";
			else if (count == 0)
				vhdlgen << "			s_r(" << outbits - outcount << ")	<= s_mux(" << outcount - 1 << "); \n";
			else
			{

				vhdlgen << "			s_r(" << outbits - outcount << ")	<= s_mux(" << outcount + muxjump - 1 << "); \n";
			}
			xorjump = bits - (outcount)+xorjump;

		}
		vhdlgen << std::endl;
	}
	vhdlgen << "		end if; \n	end process; \n"
		<< "	r <= f_barel_shifter2(s_r,s_sel); \n"
		<< "	END behavioural;";


	// Close the file stream explicitly
	vhdlgen.close();
	cout << "Document closed \n";

	// 

	// Return
	cout << "DONE!\n";
	system("pause");    // wait for a keypress
						// b_file is closed implicitly here
}