import numpy as np
import matplotlib.pyplot as plt
#import gwatpy.waveform_generator_ext as wg
#import gwatpy.plot as gp ; gp.set();
#import seaborn as sns; sns.set();
import gwatpy.mcmc_routines as gmcmc


color1= "#0D0D0D"
#color1= "white"
color2= "#254159"
whitened_data = "data/whitened_GW150914_Hanford.csv"
template_data = "data/template_GW150914_Hanford_"
image_file = "images/waveform_plot_GW150914.png"
#whitened_data = "data/whitened_GW170729_Hanford.csv"
#template_data = "data/template_GW170729_Hanford_"
#image_file = "images/waveform_plot_GW170729.png"
#whitened_data = "data/whitened_GW170608_Hanford.csv"
#template_data = "data/template_GW170608_Hanford_"
#image_file = "images/waveform_plot_GW170608.png"
templates = 1000

#fig, ax = plt.subplots(nrows=1,ncols=1,figsize=(30,6),dpi=300)
datafile = 'data/output_GW150914.hdf5'
psdfile = 'data/GWTC1_GW150914_PSDs.dat.txt'
#datastreamfile = '/Users/sperkins/Downloads/LOSC_data/GW150914/H-H1_GWOSC_4KHZ_R1-1126259447-32.txt'
datastreamfile = None
fig= gmcmc.plot_bayesogram(datafile, psdfile, "Hanford",generation_method_base='IMRPhenomD',generation_method_extended=None, threads=10, xlim = [5.8,6.1],data_stream_file=datastreamfile)
ax = fig.axes[0]

data = np.loadtxt(whitened_data,delimiter=',',unpack=True)
inter = data[1]+1j*data[2] 
inter = np.where(data[0]<512, inter,np.zeros(len(inter)))
inter = np.where(data[0]>30, inter,np.zeros(len(inter)))
df = data[0][1]-data[0][0]
#datt = np.fft.ifft(data[1]+1j*data[2])
datt = np.fft.ifft(inter)*df

T=1./(data[0][1]-data[0][0])

times = np.linspace(0,T,len(data[0]))

start = (int)(5.8/8*len(data[0]))
end = (int)(6.1/8*len(data[0]))
#start = 0
#end = len(data[0])

ax.plot(times[start:end],datt[start:end],color=color1)
xlength = len(times[start:end])
ylength = len(datt[start:end])

#x=[]
#y=[]
#for i in np.arange(templates):
#    wf = np.loadtxt(template_data+"{}.csv".format(i),delimiter=',',unpack=True)
#    wfft = np.fft.ifft(wf[1]+1j*wf[2])
#    ax.plot(times[start:end],np.real(wfft[start:end]),color=color2,alpha=.1,linewidth=1)
#    #for i in wfft[start:end]:
#    #    y.append(np.real(i))
#    #for i in times[start:end]:
#    #    x.append(i)
##z, xedges, yedges = np.histogram2d(y,x,density=True,bins=1000)
#
##ax.pcolormesh(z )
##sns.kdeplot(x,y)
##ax.hexbin(x, y, gridsize=(150,150) )


    
plt.axis('off')
plt.savefig(image_file,transparent=True)
plt.close()



##################################################################################

#T = 8
#flow=10
#fhigh=4024
#f = np.ascontiguousarray(np.linspace(flow,fhigh,(int)((fhigh-flow)*T)))
#wfs = []
#
#mass1 = 33.
#mass2 = 32.
#spin1=np.ascontiguousarray([0,.001,.1])
#spin2=np.ascontiguousarray([0,.01,.2])
#phic = 1.
##tc = T*3./4.
#tc = 6.03
#Nmod = 0
#bppe = np.ascontiguousarray([0],dtype=np.intc)
#betappe = np.ascontiguousarray([0],dtype=np.float64)
#theta=0.1
#phi=0.1
#incl_angle=0.01
#f_ref=20.
#phiRef=0.
#DL = 410.
#
#params = wg.gen_params_py(mass1,mass2,DL,spin1,spin2,phic,tc,bppe,betappe,Nmod,theta,phi,incl_angle,f_ref,phiRef,False);
#hp,hc = wg.fourier_waveform_polarizations_py(f,b'IMRPhenomD',params)
#
#hpt = np.fft.ifft(hp)/5e-24
#times = np.linspace(0,T,len(f))
#
##trim_length0=(int)(len(times)*2.95/4)
##trim_length1=(int)(len(times)*3.02/4)
#trim_length0=(int)(len(times)*5.7/8)
#trim_length1=(int)(len(times)*6.1/8)
#
#ax.plot(times[trim_length0:trim_length1],hpt.real[trim_length0:trim_length1])
