#!/usr/bin/env python
# coding: utf-8

# In[29]:


import re
f = open("C://Users//lenovo//Desktop//delay//p2p//queue_P2P_ge.txt","r")
g = open("C://Users//lenovo//Desktop//delay//p2p//filename.txt", 'w')
#print(f.read())
time_re = re.compile('\+(.+?)ms')
for line in f:
    match = time_re.match(line)
    if match is not None:
        g.write(match.group(1) + '\n')
f.close()
g.close()


# In[ ]:




