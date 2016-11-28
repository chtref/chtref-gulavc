import sys, urllib.request

def reporthook(a, b, c):
    print ("% 3.1f%% of %d bytes\r" % (min(100, float(a * b) / c * 100), c))
    sys.stdout.flush()
for url in sys.argv[1:]:
    i = url.rfind("/")
    file = url[i+1:]
    print (url, "->", file)
    urllib.request.urlretrieve(url, file, reporthook)
print