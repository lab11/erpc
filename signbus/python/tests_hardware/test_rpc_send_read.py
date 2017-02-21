#!/usr/bin/env python3

import logging
logging.basicConfig(level=logging.DEBUG)
log = logging.getLogger(__name__)

import signpost

print("Getting an edison client instance")
edison = signpost.EdisonApiClient()

print("Sending request")
edison.send_read_rpc()

