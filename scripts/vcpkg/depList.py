from itertools import chain

#will be called from server/client script
def getCommonList(args):
    base = ["boost-test", "boost-log", "boost-tti"]
    add = {"qt6.2.1": ["openssl"]}
    return chain(base, *[add[arg] for arg in filter(lambda x: x in add, args)])
