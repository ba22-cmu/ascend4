- start ubuntu (2024 is suggested)

- clone this repo: git clone git@github.com:ba22-cmu/ascend4.git

- cd ascend4 and git checkout ba-widl-0

- update emsdk in the home directory; instructions:
    https://emscripten.org/docs/getting\_started/downloads.html

- cd into this directory (tcltk/web) and run 'make' 
    There may be a few warnings.

- use 'ip addr' in ubuntu to get the vm network address seen on the windows host
    This will look something like 172.27.37.179

- in the web directory start a server 'python3 -m http.server'

- from windows browser, visit http:172.27.37.179:8000/ascjson.html
    - your ip addr goes in the url.

- hit the button

- check the console
