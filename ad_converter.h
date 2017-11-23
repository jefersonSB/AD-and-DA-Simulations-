/**
 * \file ad_converter.h
 * \brief ad_converter file.
 *
 * \author Jeferson Santos Barros <jeferson.s.barros@posgrad.ufsc.br>
 *
 * \version 1.0-dev
 *
 * \date 03/07/2017
 *
 * \defgroup main Main
 * \{
 */

/**
  * ********************************************************
  *             Core SAR  ad_converter.h
  * ********************************************************
  *
  * \class  SC_MODULE (sar)
  * \brief core of the converter
  *
  * This Module receives the signal coming from Sample and Hold
  * This Module depends on the registers: ADC12CTL0, ADC12CTL1.
  *
  * 	SAMPCO (Convert) - This bit comes from the MUX_SHP block. This should be sensitive to Convert.neg();
  *
  *
  * The analog input voltage range must be within the reference voltage range
  * selected from VR + to VR- for valid conversion results.
  *
  * Note :(VeREF+ - VREF–/VeREF-) Differential external reference voltage input
  *
  * Output result NADC = 4095 * [(Vin - (VR-)] / [(VR+) - (VR-)]
  *
  */

#ifndef AD_CONVERTER_H
#define AD_CONVERTER_H

#include <math.h>
#include <systemc.h>


class ad_converter : public sc_module {

public:

    sc_in   < bool   > triguer;               ///< \var This signal trigger the conversion (sensitive.neg())
    sc_in   < bool   > sar_clk;               ///< \var Clock input
    sc_in   < double > sample_port_in;        ///< \var Sample input

    sc_in   < double > vrp_port;              /// Positive reference input port
    sc_in   < double > vrn_port;              /// begative reference input port
    sc_out  < sc_uint < 32 > > result_port;   ///< \var Port output for octave verification

    // End

// Variable block for binary search and voltage reference selection

    double sample_value;            ///< \var Sampling voltage value
    double offset;                  ///< \var Compensation ADC

    double V_fsr;                   ///< \var Voltage range for resolution calculation
    double resol;                   ///< \var Resolution
    int n_bit;                      ///< \var Number of converter bits

    double VeREF_P;                 ///< \var Positive external reference voltage input
    double VeREF_n;                 ///< \var Negative external reference voltage input

    sc_uint < 32 > tmp_result;      ///< \var Temporary result for use in the SAR core
    sc_uint < 32 > dig_result;      ///< \var Conversion result
    sc_uint < 32 > tmp_ref;         ///< \var Temporary boost reference for use in the SAR core

// Event block
    sc_event start_conv_ev;         // Notification of sample entry
    // End

// Bloco de metodos
    void binary_search      ();     ///< \fn Performs Binary search algorithm
    void read_sample        ();     ///< \fn Performs sample reading delivered by Sample and Hold
    void Voltage_ref_sel    ();     ///< \fn Selects reference voltage
    // End

    ad_converter(sc_module_name name);
    ~ad_converter(){}
    SC_HAS_PROCESS(ad_converter);
};

# endif
