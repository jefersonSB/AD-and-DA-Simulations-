/**
 * \file ad_converter.h
 * \brief ad_converter file.
 *
 * \author Jeferson Santos Barros <jeferson.s.barros@posgrad.ufsc.br>
 *
 * \version 1.0-dev
 *
 * \date 22/11/2017
 *
 * \defgroup main Main
 * \{
 */

/**
  * ***************************************
  *         Core SAR  ad_converter.cpp
  * ***************************************
  */

#include "ad_converter.h"

ad_converter::ad_converter(sc_module_name name)
    : sc_module(name)
{
    sample_value  = 0.0;
    offset        = 0.0;
    V_fsr         = 0;
    resol         = 0.0;
    n_bit         = 8;
    VeREF_P       = 0.2;
    VeREF_n       = 0.0;
    tmp_result    = 0x0;
    dig_result    = 0x0;
    tmp_ref       = 0x0;


    SC_THREAD(binary_search);
    sensitive << sar_clk.pos();
    dont_initialize();

    SC_METHOD(read_sample);
    sensitive << triguer.neg();
    dont_initialize();
}

/**
 * \fn void ad_converter::binary_search()
 * \brief This method realizes to binary search and also do comparator function
 *
 * Binary search is synchronized with sar_clk on rising edge
 * and should last n_bits clock periods, this method also expects
 * a conversion trigger event that is performed by low edge
 * from triguer.
 *
 */
void ad_converter::binary_search(){

    while(true){


        wait (start_conv_ev);
        offset = resol/2.0; // 50% offset

        //std::cout << "sample_value  " <<  sample_value << "   resol    " << resol << std::endl;

        for(int i = n_bit-1; i>= 0; i-- ){

            wait ();                                        // Synchronization with sar_clk

            dig_result[i] = 1;                              // Forcing the output of the ADC to 1/2 V_ref
            tmp_ref = dig_result;                           // Updated reference voltage for new binary search

            if (sample_value + offset >= (double) tmp_ref*resol){   // Comparison to determine whether to retain Bit as 1
                dig_result[i] = 1;
            }else if (sample_value + offset< (double) tmp_ref*resol){        // Comparison to determine whether to retain Bit as 0
                dig_result[i] = 0;
            }
        }

        tmp_result = dig_result;                                          // Updated reference voltage for new binary search
        result_port.write(tmp_result);
        dig_result = 0;                                                   // Cleaning the reference voltage value
        wait ();                                                          // Synchronization with sar_clk
    }
}

/**
 * \fn void ad_converter::read_sample()
 * \brief This method reads a sample present in Sampale and Hold
 *  On the lower edge of triguer.
 *
 */
void ad_converter::read_sample(){

    sample_value = sample_port_in.read();           // Capturing Sample Value
    start_conv_ev.notify();
    Voltage_ref_sel();
}

/**
 * \fn void ad_converter::Voltage_ref_sel()
 * \brief This method selects the different voltage references available
 *
 */
void ad_converter::Voltage_ref_sel(){

    VeREF_P = vrp_port.read();
    VeREF_n = vrn_port.read();

//    cout << "  VeREF_P   " << VeREF_P<< "   VeREF_n   "<<  VeREF_n<< std::endl;

    V_fsr = VeREF_P - VeREF_n;             // Calculation of voltage range
    resol = V_fsr/(pow(2,n_bit));          // Calculation of resolution
}
