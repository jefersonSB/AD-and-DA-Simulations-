
/**
 * \file sample.h
 * \brief sample file.
 *
 * \author Jeferson Santos Barros <jeferson.s.barros@posgrad.ufsc.br>
 *
 * \version 1.0-dev
 *
 * \date 15/05/2017
 *
 * \defgroup main Main
 * \{
 */

/////////////////////////////////////////////
/// Module Sample and Hold ADC12-MSP430F249 |
/////////////////////////////////////////////
/**
 * \class SC_MODULE(sample)
 *
 * \brief This module is controller by SAMPCOM in two modes (Extended and Pulse)
 *
 *   When SAMPCON = 1 Sample and Hold active and sample the signal with the ADC12CLK,
 *   When SAMPCON = 0 Sample disabled SAMPCON comes from blo MUX_SHP
 *
 * Resolution (LSB) = Vmax / (2 ^ n) = 3.3V / 4095 = 805.86 uV
 *     The VC voltage of the IC capacitor must be charged within 1/2 LSB
 *     Of the VS source voltage for accurate 12-bit conversion.
 *
 */

#include <systemc-ams.h>
#include <cmath>

//SC_MODULE(sample){

class sample : public sc_module {

public:

    sca_tdf::sca_in < bool   > ctrl_lp;				// CAPCOM input for the switch
    sca_tdf::sca_in < double > in_src;				// Analog signal input from the MUX
    sc_out          < double > out;				// Sampled signal output

    sca_eln::sca_tdf_rswitch key;				// Switch ideal. Simulates sample switching.

    sca_eln::sca_node n1,n2,n3,n4;				// electrical nodes
    sca_eln::sca_node_ref gnd;

    sca_eln::sca_c CI;						// Low-pass filter input capacitance
    sca_eln::sca_r RI;						// Internal filter resistance low pass filter
    sca_eln::sca_r RS;						// External source resistance

//    sca_eln::sca_de::sca_vsource vin_src;			// Voltage source from MUX
    sca_eln::sca_tdf::sca_vsource vin_src;			// Voltage source from MUX
    sca_eln::sca_de::sca_vsink vout;				// Output voltage sampled

    //    SC_CTOR(sample)
    sample(sc_module_name name)
        : sc_module(name)
        , CI("CI"),RI("RI")
        , vin_src("vin_src")
        , vout("vout")
        , RS ("RS")
        , key("key")

    {
        double Rs = 0e3; 				// choose fixed R
        double Ri = 2e3; 				// choose fixed R
        double C = 10e-12;
        //double C = 1.989436788648692e-09;

        vin_src.inp(in_src);
        vin_src.p(n1);
        vin_src.n(gnd);

        RS.value = Rs;
        RS.n(n1);
        RS.p(n2);

        RI.value = Ri;
        RI.n(n2);
        RI.p(n3);

        key.p(n4);
        key.n(n3);
        key.ctrl(ctrl_lp);


        key.ron;
        key.roff;

        CI.value = C;
        CI.p(n4);
        CI.n(gnd);

        double t = 5.0*((Rs+Ri)*C);
        double tsample =((Rs+Ri)*(9.011)*C + 800e-9);				// Specification of sample time in relation to Rs
        double freq_c = 1/(2*M_PI*(Ri+Rs)*C);

        cout <<'\n' << '\t' << "5*Tau " << t << endl;
        cout << '\t' << "Frequência de Corte:  " << freq_c << endl;
        cout <<  '\t' << "tsample >= " << tsample << endl;

        vout.outp(out);
        vout.p(n4);					// Vout.tdf_voltage(out);
        vout.n(gnd);
    }
    ~sample(){}

};
