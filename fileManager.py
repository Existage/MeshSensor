#need to generate client_secrets.json file from googledrive api and place in same directory
#Will generate mycreds.txt after successful authentication
#Note, authentication token exprires after 7 days, currently not automatically refreshing access.
from pydrive.auth import GoogleAuth
from pydrive.drive import GoogleDrive
import os
import time

gauth = GoogleAuth()
# Try to load saved client credentials
gauth.LoadCredentialsFile("mycreds.txt")
if gauth.credentials is None:
    # Authenticate if they're not there
    gauth.LocalWebserverAuth()
elif gauth.access_token_expired:
    # Refresh them if expired
    gauth.Refresh()
else:
    # Initialize the saved creds
    gauth.Authorize()
# Save the current credentials to a file
gauth.SaveCredentialsFile("mycreds.txt")

drive = GoogleDrive(gauth)

folderID = '14j4tCHBmUnwBMuG9IejFZ_jySaATf8Fo' #GDfolder id

dataDir = os.getcwd() + "/data" #full path to data directory.


driveFileList = drive.ListFile({'q' : "'{}' in parents and trashed=false".format(folderID)}).GetList()

driveFileNames = []

for file in driveFileList:
    driveFileNames.append(file['title'])

#print(driveFileNames)

for localFile in os.listdir(dataDir):
    if time.strftime("%d_%m_%Y") in localFile or localFile in driveFileNames:
        pass
    else:
        filepath = os.path.join(dataDir, localFile)
        gFile = drive.CreateFile({'parents' : [{'id' : folderID}], 'title' : localFile})
        gFile.SetContentFile(filepath)
        gFile.Upload()
        
        
    
 

