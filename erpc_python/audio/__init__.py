from __future__ import print_function

import sys
import math
import random
import erpc

import audio

def get_services():
    class AudioServiceHandler(audio.interface.IAudio):
        def process_buf(self, data):
            #loop through the data adding up the bins
            vals = [0]*7
            for i in range(0,len(data)):
                vals[i%7] = vals[i%7] + data[i]
            print vals
            return 1

    AudioHandler = AudioServiceHandler()
    audioService = audio.server.AudioService(AudioHandler)
    return [audioService]

