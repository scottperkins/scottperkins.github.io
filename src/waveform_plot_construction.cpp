#include <iostream>
#include <gwat/waveform_generator.h>
#include <gwat/waveform_util.h>
#include <gwat/detector_util.h>
#include <gwat/util.h>
#include <gwat/io_util.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <omp.h>


int main(int argc, char *argv[])
{
	//int num_detectors = 2, psd_length = 16064;
	//std::string *detectors = new std::string[num_detectors];
	//std::string whitened_data_file = "data/whitened_GW170608_Hanford.csv";
	//std::string template_data_file = "data/template_GW170608_Hanford_";
	//std::string mcmc_output_file="data/output_GW170608.csv";
	//std::string psd_file = "/Users/sperkins/Downloads/LOSC_data/GW170608/GWTC1_GW170608_PSDs.dat";
	//detectors[0] = "Hanford";
	//detectors[1] = "Livingston";
	//double gps_time = 1180922494.5;//TESTING -- gw150914
	//std::string *detector_files = new std::string[num_detectors];
	//detector_files[0] =  "/Users/sperkins/Downloads/LOSC_data/GW170608/H-H1_GWOSC_4KHZ_R1-1180922479-32.txt";
	//detector_files[1] =  "/Users/sperkins/Downloads/LOSC_data/GW170608/L-L1_GWOSC_4KHZ_R1-1180922479-32.txt";

	//int num_detectors = 3, psd_length = 4016;
	//std::string *detectors = new std::string[num_detectors];
	//std::string whitened_data_file = "data/whitened_GW170729_Hanford.csv";
	//std::string template_data_file = "data/template_GW170729_Hanford_";
	//std::string mcmc_output_file="data/output_GW170729.csv";
	//std::string psd_file = "/Users/sperkins/Downloads/LOSC_data/GW170729/GWTC1_GW170729_PSDs.dat.txt";
	//detectors[0] = "Hanford";
	//detectors[1] = "Livingston";
	//detectors[2] = "Virgo";
	//double gps_time = 1185389807.3;//TESTING -- gw150914
	//std::string *detector_files = new std::string[num_detectors];
	//detector_files[0] =  "/Users/sperkins/Downloads/LOSC_data/GW170729/H-H1_GWOSC_4KHZ_R1-1185389792-32.txt";
	//detector_files[1] =  "/Users/sperkins/Downloads/LOSC_data/GW170729/L-L1_GWOSC_4KHZ_R1-1185389792-32.txt";
	//detector_files[2] =  "/Users/sperkins/Downloads/LOSC_data/GW170729/V-V1_GWOSC_4KHZ_R1-1185389792-32.txt";

	int num_detectors = 2, psd_length = 8032;
	std::string *detectors = new std::string[num_detectors];
	std::string whitened_data_file = "data/whitened_GW150914_Hanford.csv";
	std::string template_data_file = "data/template_GW150914_Hanford_";
	std::string mcmc_output_file="data/output_GW150914.csv";
	std::string psd_file = "/Users/sperkins/Downloads/LOSC_data/GW150914/GWTC1_GW150914_PSDs.dat.txt";
	detectors[0] = "Hanford";
	detectors[1] = "Livingston";
	double gps_time = 1126259462.4;//TESTING -- gw150914
	std::string *detector_files = new std::string[num_detectors];
	detector_files[0] =  "/Users/sperkins/Downloads/LOSC_data/GW150914/H-H1_GWOSC_4KHZ_R1-1126259447-32.txt";
	detector_files[1] =  "/Users/sperkins/Downloads/LOSC_data/GW150914/L-L1_GWOSC_4KHZ_R1-1126259447-32.txt";
	int datalength = 131075;

 	double trigger_time = gps_time;
	double **psd_data = allocate_2D_array(num_detectors,psd_length);
	double **freqs_data = allocate_2D_array(num_detectors,psd_length);
	std::complex<double> **data = (std::complex<double> **)malloc(sizeof(std::complex<double> *)*num_detectors);
	for(int i =0; i<num_detectors; i++)
		data[i] = (std::complex<double>*)malloc(sizeof(std::complex<double>)*psd_length);

	allocate_LOSC_data(detector_files, psd_file, num_detectors, psd_length, datalength, trigger_time, data, psd_data, freqs_data);

	double Tobs = 1./(freqs_data[0][1]-freqs_data[0][0]);
	double **temp = allocate_2D_array(psd_length,3);
	
	for(int i=0 ; i<psd_length; i++){
		temp[i][0]=freqs_data[0][i];	
		temp[i][1]=std::real( data[0][i]/sqrt(psd_data[0][i]));
		temp[i][2]=std::imag( data[0][i]/sqrt(psd_data[0][i]));
	}
	write_file(whitened_data_file,temp,psd_length,3);
	deallocate_2D_array(temp,psd_length,3);
	

	//###############################################################
	int length;
	count_lines_data_file(whitened_data_file,&length);
	double **whitened_data = allocate_2D_array( length, 3);
	read_file(whitened_data_file,whitened_data,length,3);
	double freqs[length];
	for(int i =0; i <length; i++){
		freqs[i]=whitened_data[i][0];
	}
	int samples;
	count_lines_data_file(mcmc_output_file,&samples);
	double **mcmc_output = allocate_2D_array( samples, 14);
	read_file(mcmc_output_file,mcmc_output,samples,14);
	
	std::complex<double> response[length];
	int waveform_num = 1000;
	double rand_nums[waveform_num];
	gsl_rng_env_setup();
	const gsl_rng_type *T=gsl_rng_default;
	gsl_rng * r = gsl_rng_alloc(T);
	gsl_rng_set(r,10);
	for(int i = 0 ; i<waveform_num; i++){
		rand_nums[i]=gsl_rng_uniform(r);
	}
	gsl_rng_free(r);	
	double psd[length];
	populate_noise(freqs, "Hanford_O1_fitted",psd,length);

	
	omp_set_num_threads(10);
	#pragma omp parallel for
	for(int i = 0 ; i<waveform_num; i++){
		//int sample = samples*rand_nums[i];
		int sample = i;
		double **output = allocate_2D_array(length,3);

		gen_params params;
		params.mass1 = calculate_mass1(std::exp(mcmc_output[sample][7] ),mcmc_output[sample][8]);
		params.mass2 = calculate_mass2(std::exp(mcmc_output[sample][7] ),mcmc_output[sample][8]);
		params.RA = mcmc_output[sample][0];
		params.DEC = asin(mcmc_output[sample][1]);
		params.psi = mcmc_output[sample][2];
		params.incl_angle = acos(mcmc_output[sample][3]);
		params.phiRef = mcmc_output[sample][4];
		params.tc = Tobs-mcmc_output[sample][5];
		params.Luminosity_Distance = std::exp(mcmc_output[sample][6]);
		double spin1[3];double spin2[3];
		spin1[0]=mcmc_output[sample][9]*sin(acos(mcmc_output[sample][11]))*cos(mcmc_output[sample][13]);
		spin2[0]=mcmc_output[sample][10]*sin(acos(mcmc_output[sample][12]));
		spin1[1]=mcmc_output[sample][9]*sin(acos(mcmc_output[sample][11]))*sin(mcmc_output[sample][13]);
		spin2[1]=0;
		spin1[2]=mcmc_output[sample][9]*mcmc_output[sample][11];
		spin2[2]=mcmc_output[sample][10]*mcmc_output[sample][12];
		//params.spin1[2]=mcmc_output[sample][9];
		//params.spin2[2]=mcmc_output[sample][10];
		//params.spin1[2]=mcmc_output[sample][9];
		//params.spin2[2]=mcmc_output[sample][10];
		params.NSflag1=false;
		params.NSflag2=false;
		params.equatorial_orientation=false;
		params.horizon_coord=false;
		params.shift_time=true;
		params.shift_phase=true;
		params.gmst = gps_to_GMST_radian(gps_time);
		params.f_ref=20;
		fourier_detector_response(freqs,length,response, "Hanford","IMRPhenomPv2",&params,(double *)NULL);

		for(int k = 0 ; k<length; k++){
			output[k][0]=freqs[k];
			output[k][1]=real(response[k])/sqrt(psd_data[0][k]);
			output[k][2]=imag(response[k])/sqrt(psd_data[0][k]);
		}
		write_file(template_data_file+std::to_string(i)+".csv",output, length,3);
	
		deallocate_2D_array(output,length,3);
	}
	
	deallocate_2D_array(whitened_data,length,3);
	deallocate_2D_array(mcmc_output,samples,11);
	deallocate_2D_array(psd_data,num_detectors, psd_length);
	deallocate_2D_array(freqs_data,num_detectors, psd_length);
	for(int i =0; i<num_detectors; i++)
		free(data[i]);
	free(data);
	delete [] detector_files;
	return 0;
}
