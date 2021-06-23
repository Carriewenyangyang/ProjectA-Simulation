#!/usr/bin/env python
# coding: utf-8

# In[33]:


import re
f = open("C://Users//lenovo//Desktop//PacketSize_Time.txt","r")
g = open("C://Users//lenovo//Desktop//PacketSize.txt", 'w')
z = open("C://Users//lenovo//Desktop//Time.txt", 'w')
#print(f.read())
time_re = re.compile('(\d*)(\s*)\+(.+?)ns')
for line in f:
    match = time_re.match(line)
    if match is not None:
        g.write(match.group(1) + '\n')
        z.write(match.group(3) + '\n')
f.close()
g.close()
z.close()


# In[ ]:





# In[ ]:




