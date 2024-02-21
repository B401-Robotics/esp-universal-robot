import re
import json
from re import sub


# src_name = "assets/index-4sK4E3Wk.css"
# src_name = "assets/index-1f4CyHxp.js"

# new_name = re.sub(r'.*/([^.-]+).*\.(.*)', r'/\1.\2', src_name)
# print(new_name)

data_json = '''
{
  "index.html": {
    "assets": [
      "assets/react-h3aPdYU7.svg"
    ],
    "css": [
      "assets/index-4sK4E3Wk.css"
    ],
    "file": "assets/index-1f4CyHxp.js",
    "isEntry": true,
    "src": "index.html"
  },
  "src/assets/react.svg": {
    "file": "assets/react-h3aPdYU7.svg",
    "src": "src/assets/react.svg"
  }
}
'''

data = json.loads(data_json)

for key in data['index.html']:
    print(key)
    print(data['index.html'][key])

    if key == 'isEntry':
        continue

    if type(data['index.html'][key]) == list:
        src_name = data['index.html'][key][0]
    else:
        src_name = data['index.html'][key]

    # src_name = data['index.html'][key]
    dst_name = re.sub(r'.*/([^.-]+).*\.(.*)', r'/\1.\2', src_name)
    
    print("REPLACED: " + src_name + " -> " + dst_name)
    # print("SRC: " + src_name)
    print('---')