

#ifndef DA_CONVERTER_H
#define DA_CONVERTER_H

# include <systemc-ams.h>

//using namespace std;

SCA_TDF_MODULE (da_converter) {

    sca_tdf::sca_out <double> vout_port;
    sca_tdf::sca_in  <double> v_in_p_port;
    sca_tdf::sca_in  <double> v_in_n_port;
    sca_tdf::sc_in   < sc_uint <32> > digital_word_pot;

    da_converter( sc_core::sc_module_name nm, int n_b,sca_core::sca_time Tm_)
    {
        Tm = Tm_;
        v_in_p = 0;
        v_in_n = 0;
        n_bits = n_b;
        resolution = resolution;
    }

    void set_attributes();
    void processing();

    private:
        double t, v_in_p, v_in_n, resolution, vout;
        int n_bits;
        sca_core::sca_time Tm; // module time step
};
#endif // srcs_H
