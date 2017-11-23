


# include"da_converter.h"



void da_converter::set_attributes() {
    set_timestep(Tm);
}

void da_converter::processing() {

    v_in_p = v_in_p_port.read();
    v_in_n = v_in_n_port.read();
    resolution = (v_in_p - v_in_n)/(pow(2,n_bits));

//    cout << "v_in_p   " << v_in_p << "  v_in_n  " <<  v_in_n << std::endl;

    cout << "Output AD = " << digital_word_pot.read() << "  Output DA  " <<  digital_word_pot.read()*resolution << std::endl;

    vout_port.write(digital_word_pot.read()*resolution);
    vout = 0;
}
