from platformio import util
import os 

Import("env")

with open("otaAuth.txt") as f:
  auth = f.readlines()[0].strip()

  env.Append(UPLOAD_FLAGS=[
    "--auth=" + auth
  ])
