#!/usr/bin/env python
#########################################################
#Author  : Arafat NOUREDDINE
#Date    : 2016/08/28
#Purpose : Test XiaDxp (Mode MCA)
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
  print "Usage: [python] test_xiadxp_mca.py <my/device/proxy> <configAlias> <presetValue>"
  print '\n',
  sys.exit(1)


#------------------------------------------------------------------------------
# run
#------------------------------------------------------------------------------
def run(proxy_name = 'arafat/xia/xiadxp', config_alias='1', preset_value=5):
    # print arguments
    print '==================================================\n',
    print 'program inputs :'
    print 'proxy_name\t = '     , proxy_name
    print 'config_alias\t = '   , config_alias
    print 'preset_value\t = '   , preset_value


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
        proxy.presetValue       = float(preset_value)

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
        state = proxy.state()
        if (state!=PyTango.DevState.STANDBY):
            status = proxy.status()
            raise Exception('state : (%s)\nstatus : (%s)' %(state , status))
        print '\n',

        #check duration of acquisition
        duration = (timeEnd-timeBegin).total_seconds()
        #allow +10%
        if(duration>(float(preset_value)+0.1*float(preset_value))):
            raise Exception('timing too long : (%s) > (%s)' %(duration , preset_value))
        #allow -10%
        if(duration<(float(preset_value)-0.1*float(preset_value))):
            raise Exception('timing too short : (%s) < (%s)' %(duration , preset_value))


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
    if len(sys.argv) !=4 :
        usage()
    print run(*sys.argv[1:])
