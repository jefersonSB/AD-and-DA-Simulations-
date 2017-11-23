# include"srcs_sca.h"

void srcs_sca::set_attributes() {
    set_timestep(Tm);
}

void srcs_sca::processing() {
    t = get_time().to_seconds(); // actual time

    if(offset == true){
        out.write( (ampl/2.0) * sin( 2.0 * M_PI * freq * t ) + (ampl/2.0));
    }else {
        out.write( (ampl * sin( 2.0 * M_PI * freq * t )) + ampl);
    }
}
