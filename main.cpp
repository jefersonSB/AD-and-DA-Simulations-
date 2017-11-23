

#include <systemc.h>
#include <systemc-ams.h>


#include "srcs.h"
#include "srcs_sca.h"
#include "ad_converter.h"
#include "da_converter.h"
#include "drive_conv_port.h"
#include "sample.h"
#include "filter.h"



int sc_main(int argn, char* argc[])              // SystemC main program
{
    sca_core::sca_time Tm_ = sca_core::sca_time(1.0, sc_core::SC_NS);

    sc_clock clk("clk", 5, SC_US);               // Clock for ADC and DAC
    sc_clock clk_sar("clk_sar", 0.2, SC_US);     // Clock for ADC SAR algorithm


    sca_tdf::sca_signal < double > imput_sample; //
    sc_signal < double > sig_vrp;                // Signal to connect positive source for AD
    sc_signal < double > sig_vrn;                // Signal to connect negative source for AD


    sca_tdf::sca_signal < double > vout_da_sig;   // Output DA
    sca_tdf::sca_signal < double > vp_da_ref;     // Signal to connect negative source for DA
    sca_tdf::sca_signal < double > vn_da_ref;     // Signal to connect negative source for ad


    sc_signal < sc_uint < 32 > > digital_word;   // Signal
    sc_signal <double> sample_sig;               // Signal


    sca_tdf::sca_signal <bool> sampcon_tdf_sig;  // Adaptor Signal switch Sample And Hold
    sc_signal <bool> sampcon_de_sig_out;         // Adaptor Signal Triguer SAR


// Source block
    ///Source input signal to converter
    srcs_sca srcs_sca_1 ("srcs_sca_1", 3.0, 2e3, true, Tm_);
    srcs_sca_1.out(imput_sample);


    //Positive reference signal source AD
    srcs src_2 ("source2", 3.0, 0e3, Tm_ );      // Instantiate source
    src_2.out_tdf_de(sig_vrp);

    //Negative reference signal source AD
    srcs src_3 ("source3", 0.0, 0e3, Tm_ );      // Instantiate source
    src_3.out_tdf_de(sig_vrn);


    ///Negative reference signal source DA
    srcs_sca srcs_sca_2 ("srcs_sca_2", 3.0, false, 0e3, Tm_);
    srcs_sca_2.out(vp_da_ref);

    ///Negative reference signal source DA
    srcs_sca srcs_sca_3 ("srcs_sca_3", 0.0, 0e3, false, Tm_);
    srcs_sca_3.out(vn_da_ref);
    // End


// Drive for port conversion between domains
    drive_conv_port drive_conv_port_1 ("drive_conv_port_1", Tm_);
    drive_conv_port_1.sampcon_tdf_port (sampcon_tdf_sig);
    drive_conv_port_1.sampcon_de_port_out(sampcon_de_sig_out);
    drive_conv_port_1.sampcon_de_por_0 (clk);             // For machine conseq 00 and SHP = 0
// End

// Sample and hold block
    sample *sample_1 = new sample("sample1");
    sample_1->in_src(imput_sample);
    sample_1->ctrl_lp(sampcon_tdf_sig);
    sample_1->out(sample_sig);
    // End

// ad_converter (SAR block)
    ad_converter *ad_converter_1 = new ad_converter("ad_converter_1");
    ad_converter_1->sample_port_in (sample_sig);
    ad_converter_1->result_port (digital_word);
    ad_converter_1->triguer (sampcon_de_sig_out);
    ad_converter_1->sar_clk (clk_sar);
    ad_converter_1->vrp_port (sig_vrp);
    ad_converter_1->vrn_port (sig_vrn);
    // End



//da_converter
    da_converter da_converter_1("da_converter_1", 8, Tm_);
    da_converter_1.digital_word_pot(digital_word);
    da_converter_1.v_in_p_port(vp_da_ref);
    da_converter_1.v_in_n_port(vn_da_ref);
    da_converter_1.vout_port(vout_da_sig);
    // End

//Filter recontruct signal
    sca_tdf::sca_signal <double> recontruct_signal;  // Adaptor Signal switch Sample And Hold

    filter filter_1("filter_1", 50.0e3, 1.0, Tm_);
    filter_1.in(vout_da_sig);
    filter_1.out(recontruct_signal);

//Filter recontruct signal
        sca_tdf::sca_signal <double> recontruct_signal_2;  // Adaptor Signal switch Sample And Hold

        filter filter_2("filter_2", 10.0e3, 1.0, Tm_);
        filter_2.in(vout_da_sig);
        filter_2.out(recontruct_signal_2);


    sca_trace_file* tfp = sca_create_tabular_trace_file("testbench");

        sca_trace(tfp, imput_sample, "imput_sample");                 // Define which signal to trace
//        sca_trace(tfp,sample_sig , "output_sample");
        sca_trace(tfp,digital_word , "Output_AD_input_DA");
//        sca_trace(tfp,vout_da_sig , "vout_da_sig");
        sca_trace(tfp,recontruct_signal , "recontruct_signal");
        sca_trace(tfp,recontruct_signal_2 , "recontruct_signal_2");
//        sca_trace(tfp,sampcon_tdf_sig , "sampcon_tdf_sig");


    sc_trace_file *wf = sc_create_vcd_trace_file("counter");
        wf -> set_time_unit(1.0, sc_core::SC_NS);

        sc_trace(wf, clk, "Triguer_clk");
        sc_trace(wf,clk_sar , "clk_sar");
        sc_trace(wf, digital_word, "digital_word");

        //sc_trace(wf,sampcon_de_sig_out , "sampcon_de_sig_out");


    sc_start(0.5, SC_MS);                     // Start simulation for 100 ms
    sca_close_tabular_trace_file(tfp);         // Close trace file
    sc_close_vcd_trace_file(wf);

    return 0;                                  // Exit with return code 0
}
