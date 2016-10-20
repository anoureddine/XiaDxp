#!/usr/bin/env python
#########################################################
#Author  : Arafat NOUREDDINE
#Date    : 2016/08/28
#Purpose : Test XiaDxp (Mode MAPPING)
#########################################################
import os
import sys
import PyTango
import time
import datetime

proxy = ''
#------------------------------------------------------------------------------
# build exception
#------------------------------------------------------------------------------
class BuildError(Exception):
  pass


#------------------------------------------------------------------------------
# Usage
#------------------------------------------------------------------------------
def usage():
  print '\n',
  print "Usage: [python] test_xiadxp_mapping.py <my/device/proxy> <configAlias> <nbPixels> <streamType> <streamPath> <streamFile>"
  print '\n',
  sys.exit(1)


#------------------------------------------------------------------------------
# run
#------------------------------------------------------------------------------
def run(proxy_name = 'arafat/xia/xiadxp', config_alias='2', nb_pixels=500, stream_type='nexus_stream', stream_path="c:\\arafat\\nxs\\xiadxp", stream_file='test_'):
    # print arguments
    print '==================================================\n',
    print 'program inputs :'
    print 'proxy_name\t = '     , proxy_name
    print 'config_alias\t = '   , config_alias
    print 'nb_pixels\t = '      , nb_pixels
    print 'stream_type\t = '    , stream_type
    print 'stream_path\t = '    , stream_path
    print 'stream_file\t = '    , stream_file


    #Configure the device
    print '==================================================\n',
    print 'create the device proxy :'
    timeBegin = datetime.datetime.now()
    print '\r',timeBegin.isoformat(), " - create a DeviceProxy to device ", proxy_name
    proxy = PyTango.DeviceProxy(proxy_name)
    timeBegin = datetime.datetime.now()
    print '\r',timeBegin.isoformat(), " - ping proxy(\"",proxy_name,"\") ",proxy.ping()," us"
    print '\n'

    try:

        print '==================================================\n',
        #Display time when state is STANDBY (just before load config)
        timeBegin = datetime.datetime.now()
        print timeBegin.isoformat(), ' - ', proxy.state()

        #Load config file
        print 'Load config file : ', config_alias
        proxy.LoadConfigFile(config_alias)

        #Loop while state is DISABLE
        state = proxy.state()
        while (state==PyTango.DevState.DISABLE):
            state = proxy.state()
            if state == PyTango.DevState.STANDBY:
                break
            print '\r', 'DISABLE ...',
            time.sleep(0)

        #Display time when state is STANDBY (just after acquisition is finish)
        timeEnd = datetime.datetime.now()
        print '\n', timeEnd.isoformat(), ' - ', proxy.state()
        print 'duration LoadConfigFile : ', ((timeEnd-timeBegin).total_seconds()*1000),'(ms)'
        #check integrity state after the previous action
        state = proxy.state()
        if (state!=PyTango.DevState.STANDBY):
            raise Exception('FAIL : Load is end with state (%s)' %(state))

        print '\n'
        print 'nbModules = ',proxy.nbModules
        print 'nbChannels = ',proxy.nbChannels
        print 'nbBins = ',proxy.nbBins
        print 'currentAlias = ',proxy.currentAlias
        print 'currentMode = ',proxy.currentMode
        print '\n'

        print '==================================================\n',
        proxy.nbPixels          = long(nb_pixels)
        proxy.streamType        = stream_type
        proxy.streamTargetPath  = stream_path
        proxy.streamTargetFile  = stream_file

        #Display time when state is STANDBY (just before snap)
        timeBegin = datetime.datetime.now()
        print timeBegin.isoformat(), ' - ', proxy.state()

        #Snap command
        print 'Snap'
        proxy.Snap()

        #Loop while state is DISABLE
        state = proxy.state()
        while (state==PyTango.DevState.RUNNING):
            state = proxy.state()
            if state == PyTango.DevState.STANDBY:
                break
            print '\r', 'RUNNING ...',
            time.sleep(0)

        #Display time when state is STANDBY (just after acquisition is finish)
        timeEnd = datetime.datetime.now()
        print '\n', timeEnd.isoformat(), ' - ', proxy.state()
        print 'duration Snap : ', ((timeEnd-timeBegin).total_seconds()*1000),'(ms)'

        #check integrity state after the previous action
        time.sleep(2);#nxs error occured later
        state = proxy.state()
        if (state!=PyTango.DevState.STANDBY):
            status = proxy.status()
            raise Exception('state : (%s)\nstatus : (%s)' %(state , status))
        print '\n',

        print 'currentPixel = ', proxy.currentPixel
        if long(proxy.currentPixel)!=long(nb_pixels):
            raise Exception('Acquired pixels (%s) is different from requested nb_pixels (%s)'  % (proxy.currentPixel, nb_pixels))

        print '==================================================\n',
        print 'program has completed normally\n',
        print '==================================================\n',
    except Exception as err:
        print '\n--------------------------------------------------\n',
        print 'ERROR : \n',
        sys.stderr.write('%s' %err)
        print '\n--------------------------------------------------\n',

#------------------------------------------------------------------------------
# Main Entry point
#------------------------------------------------------------------------------
if __name__ == "__main__":
    if len(sys.argv) !=7 :
        usage()
    print run(*sys.argv[1:])
