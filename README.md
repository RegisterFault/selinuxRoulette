# selinuxRoulette
A random selinux label changing program.

***IMPORTANT:*** RUN THIS CODE AT YOUR OWN RISK. IT DOES VERY STUPID THINGS TO THE LIVE RUNNING
SYSTEM ROOT FILESYSTEM THAT MIGHT RESULT IN EXTREMLY UNPREDICTABLE BEHAVIOR. THE CHANGES BEING 
MADE BY THIS PROGRAM ARE INTENDED TO BE NON-PERMANENT, BUT IN MAKING THESE CHANGES, IT CAN AND WILL
DISRUPT SYSTEM SERVICES IN UNPREDICTABLE WAYS AND POTENTIALLY RESULT IN DATA LOSS, CORRUPTION, AND 
BEING LOCKED OUT OF THE SYSTEM. DO NOT RUN THIS ON A SYSTEM YOU NEED TO RELY UPON.

This will nonpermanently change a random selection of system files and dev nodes to have the
selinux label of `default_t`, which will likely cause subtle chaos in the
currently running system.

Because of the nuances of how selinux works, this will VERY likely have no
noticeable effect for the majority of its operation, even after changing a
truly absurd number of file labels, but if you have a lot of background services
running, this could subtly disrupt their operations.

This is a SUBSTANTIALLY different situation if you run this on a webserver
running selinux, as apache httpd is heavily moderated and constrained by 
selinux.

To reverse the effects of this program, you should be able to trigger an selinux
filesystem relabel, as this program does not modify selinux policy.

This can be performed buy either executing:

```
touch /.autorelabel
sudo reboot
```

or 

```
fixfiles relabel
```

SelinuxRoulette is only going to affect anything on distros that use SELinux, which is
going to be stuff thats Red Hat/Fedora and SUSE based (except for Nobara, those guys
seemed to have looked at the lovecraftian horrors of SELinux policy and decided that
maybe they won't have any fun maintaining that). 
