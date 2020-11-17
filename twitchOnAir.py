import requests
import time
from twitchAPI.twitch import Twitch
import socket

#Thanks Inzer Lee!:
#https://stackoverflow.com/a/64442969

client_id = "Nope..."
client_secret = "NOOOOOPE."

twitch = Twitch(client_id, client_secret)
twitch.authenticate_app([])

TWITCH_STREAM_API_ENDPOINT_V5 = "https://api.twitch.tv/kraken/streams/{}"

API_HEADERS = {
    'Client-ID' : client_id,
    'Accept' : 'application/vnd.twitchtv.v5+json',
}

def checkUser(user): #returns true if online, false if not
    userid = twitch.get_users(logins=[user])['data'][0]['id']
    url = TWITCH_STREAM_API_ENDPOINT_V5.format(userid)
    try:
        req = requests.Session().get(url, headers=API_HEADERS)
        jsondata = req.json()
        #print(req.text)
        if 'stream' in jsondata:
            if jsondata['stream'] is not None:
                return True
            else:
                return False
    except Exception as e:
        print("Error checking user: ", e)
        return False

#print(checkUser('agcstream'))

if __name__ == "__main__":

    print(time.ctime(), "startup!")


    TCP_IP = socket.gethostbyname(socket.gethostname())

    TCP_PORT = 1234
    BUFFER_SIZE = 1024  # Normally 1024, but we want fast response

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)  # make socket reuseable, for debugging (enables you to rerun the program before the socket has timed out)
    s.bind((TCP_IP, TCP_PORT))
    s.listen(1)

    print('Done.. Opening TCP port', TCP_PORT)

    while True:
        try:
            conn, addr = s.accept()
            print(time.ctime(), 'Connection from:', addr)
            rx = conn.recv(BUFFER_SIZE).decode('ASCII').rstrip('\n')
            #rx = conn.rstrip('\n')
            print('RX:',rx)

            report = str(int(checkUser(str(rx))))+'\n'
            conn.send(report.encode('ASCII'))  # echo
            print('TX:', report)

            print('Closing connection')
            conn.shutdown(socket.SHUT_RDWR)
            conn.close()

            print('--------------------------')


        except Exception as e:
            # print "hmm.. It looks like there was an error: " + str(e)
            print(time.ctime(), 'Client disconnected... :', str(e))
            print('--------------------------')
            conn.close()
