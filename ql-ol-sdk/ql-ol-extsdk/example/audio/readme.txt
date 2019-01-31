
Description:
---------------
This example demonstrates how to record MIC voice, and how to get the record pcm stream data. And how to play the pcm stream data or audio file.

header file: ql_audio.h
audio play and record need two lib: lib_audio.a libalsa_intf.so


Example usage:
---------------
  Play the specified audio file: 
       <process> play1 <audio file>
  Mixer player:
       <process> play2 <audio file1> <audio file2>
  Record and playback (without saving record data):
       <process> recd1
  Record and playback (save record data to file):
       <process> recd2 <file name>
