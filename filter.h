#ifndef LFT_ND_FILTER_H
#define LFT_ND_FILTER_H
#include <systemc-ams.h>

SCA_TDF_MODULE (filter){

    sca_tdf::sca_in<double> in;				// Entrada de sinal
    sca_tdf::sca_out<double> out;			// saída

    double fc; // 3dB cut-off frequency in Hz
    double h0; // DC gain
    sca_core::sca_time Tm; // module time step

    filter( sc_core::sc_module_name nm, double fc_, double h0_, sca_core::sca_time Tm_)

            //        SCA_CTOR(filter)
        : in("in")
        , out("out")
        , fc(fc_)
        , h0(h0_)
        , Tm(Tm_)
    {}

    void initialize(){
        num(0) = 1.0;
        den(0) = 1.0;
        den(1) = 1.0 /( 2.0 * M_PI * fc );
    }

    void set_attributes() {
        set_timestep(Tm);
    }

    void processing(){
        out.write( ltf_nd( num, den, in.read(), h0 ));
    }

    private:
        sca_tdf::sca_ltf_nd ltf_nd; 			// Laplace transfer function
        sca_util::sca_vector<double> num, den;  // numerator and denominator coefficients
};

#endif
