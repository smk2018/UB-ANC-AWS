# UB-ANC-AWS
UB-ANC deployment in AWS

To avoid compatibility problem about installing VirtualBox or Ubuntu 16.04 in Windows or MacOS, we are tring to deploy several UB-ANC projects in AWS. And the following instructions describe how to setup AWS with Ubuntu 16.04 and deploy the UB-ANC projects in AWS.

## Setup Your AWS Account
First, create and activate an AWS Account. Here is the [instruction link](https://aws.amazon.com/premiumsupport/knowledge-center/create-and-activate-aws-account/?nc1=h_ls).

## Amazon EC2 Setup
We choose AWS EC2 to deploy Ubuntu 16.04. You have to setup AWS region in EC2 Dashboard first, and then click "Launch instance" button to create your first instance in AWS.
![1597779491(1)](https://user-images.githubusercontent.com/38517632/90557710-f1ee5c00-e168-11ea-9496-9a9287429bf8.png)

### Step 1: Choose an Amazon Machine Image(AMI)
An AMI is a template that contains the software configuration (operating system, application server, and applications) required to launch your instance. You can select an AMI provided by AWS, our user community, or the AWS Marketplace; or you can select one of your own AMIs.

**Choose Ubuntu Server 16.04LTS(HVM), SSD Volume Type and select 64-bit(x86). Then click "Select" button.**
![1597779664(1)](https://user-images.githubusercontent.com/38517632/90557932-472a6d80-e169-11ea-9977-6d4981a08b90.png)

### Step 2: Choose and Instance Type
Amazon EC2 provides a wide selection of instance types optimized to fit different use cases. Instances are virtual servers that can run applications. They have varying combinations of CPU, memory, storage, and networking capacity, and give you the flexibility to choose the appropriate mix of resources for your applications.

**We recommend Type m5a.large with 2 vCPUs, 8 Memory(GiB) and 1x75 SSD. Then click "Next: Configure Instance Details" button.**
![1597779748(1)](https://user-images.githubusercontent.com/38517632/90558364-f7987180-e169-11ea-8634-f737257f3d4c.png)

### Step 3: Configure Instance
Configure the instance to suit your requirements. You can launch multiple instances from the same AMI, request Spot instances to take advantage of the lower pricing, assign an access management role to the instance, and more.

**Click "Next: Add Storge" button and go to next step**
![1597779799(1)](https://user-images.githubusercontent.com/38517632/90558417-0f6ff580-e16a-11ea-9808-d1d347f441ac.png)

### Step 4: Add Storage:
Your instance will be launched with the following storage device settings. You can attach additional EBS volumes and instance store volumes to your instance, or edit the settings of the root volume. You can also attach additional EBS volumes after launching an instance, but not instance store volumes.

**You can add additional EBS volumes in this step. Just follow instructions it provides.**
![1597779839(1)](https://user-images.githubusercontent.com/38517632/90558472-26aee300-e16a-11ea-9a51-348b5181e51d.png)

### Step 5: Add Tags
A tag consists of a case-sensitive key-value pair. For example, you could define a tag with key = Name and value = Webserver.

A copy of a tag can be applied to volumes, instances or both.

Tags will be applied to all instances and volumes. 

**Follow instructions and then click "Next: Configure Security Group" button.**
![1597779879(1)](https://user-images.githubusercontent.com/38517632/90558523-35959580-e16a-11ea-8bd4-9a83ed2bf730.png)

### Step 6: Configure Security Group
A security group is a set of firewall rules that control the traffic for your instance. On this page, you can add rules to allow specific traffic to reach your instance. For example, if you want to set up a web server and allow Internet traffic to reach your instance, add rules that allow unrestricted access to the HTTP and HTTPS ports. You can create a new security group or select from an existing one below. 

**You need to click "Create a new security group" button and setup two rules:** 

__Tpye: SSH, Protocal: TCP, Port Range: 22, Source: Custom 0.0.0.0/0.__ 

__Tpye: Custom TCP Rule, Protocal: TCP, Port Range: 5901, Source: Custom 0.0.0.0/0.__ 

**Then click "Launch" button**
![1597779926(1)](https://user-images.githubusercontent.com/38517632/90558564-4940fc00-e16a-11ea-94aa-7464f29634b2.png)

### Step 7: Create a new key pair:
A security group is a set of firewall rules that control the traffic for your instance. On this page, you can add rules to allow specific traffic to reach your instance. For example, if you want to set up a web server and allow Internet traffic to reach your instance, add rules that allow unrestricted access to the HTTP and HTTPS ports. You can create a new security group or select from an existing one below. 

**Click "create a new key pair" button and setup its name. Click "Download Key Pair" button and save .pem file in your PC.**

**Then click "Choose an existing key pair" button and select the key pair you just download.**

**Next, click "Launch Instances" button. After 10s, your instances are launching and click "View Instances" button to view your first instance**

## Start to configure Ubuntu 16.04
You need to wait a few minutes for the instance to complete the initialization process. If Instance State is running and Status Checks is 2/2 checks passed, it means you can start to configure Ubuntu 16.04 Desktop.

First, choose instance and click "Connect" buttom, you can find an example command like `ssh -i "new-aws.pem" ubuntu@ec2-3-84-129-33.compute-1.amazonaws.com`

Open Terminal/Git Bash in your PC. You have to use `cd` command to find .pem file location first. Then copy `ssh -i "new-aws.pem" ubuntu@ec2-3-84-129-33.compute-1.amazonaws.com`  command **(according to the command from your EC2 instance page)** and paste it in Terminal or Git Bash. After that, you can connect to the instance:
```
cd Downloads (location where you save your .pem file)
ssh -i "new-aws.pem" ubuntu@ec2-3-84-129-33.compute-1.amazonaws.com (according to the command from your EC2 instance page)
```
![1597780232(1)](https://user-images.githubusercontent.com/38517632/90558779-a3da5800-e16a-11ea-9f59-187a8dfc854c.png)

Then you need to use several commands to setup Ubuntu 16.04 Desktop:
```
sudo apt-get upgrade
sudo apt-get install ubuntu-desktop vnc4server gnome-panel gnome-settings-daemon metacity nautilus gnome-terminal
```
Then you can use `vncserver` command. It will setup and verify a password to access your desktops.

Next, use `vncserver -kill :1` command to kill vncserver and go to the xstartup file and edit it:
```
vncserver
vncserver -kill :1
cd .vnc
sudo vim xstartup
```
In the xstartup file, press `i` in keyboard to enter INSERT mode and use `#` to comment out everything in the file. Edit the file to look like so:
```
#!/bin/sh

export XKL_XMODMAP_DISABLE=1
unset SESSION_MANAGER
unset DBUS_SESSION_BUS_ADDRESS

[ -x /etc/vnc/xstartup ] && exec /etc/vnc/xstartup
[ -r $HOME/.Xresources ] && xrdb $HOME/.Xresources
xsetroot -solid grey

vncconfig -iconic &
gnome-panel &
gnome-settings-daemon &
metacity &
nautilus &
gnome-terminal &
```
After you’re done, use `Ctrl C` to save changes, then use `SHIFT + :` and type `wq` to save and quit the file. Enter `exit` to log out.

We need to use `ssh -L 5901:localhost:5901 -i "new-aws.pem" ubuntu@ec2-3-84-129-33.compute-1.amazonaws.com` command
**(do not directly paste this command in this page, remember to check it in your EC2 instance page)** 
and connect to your instance again.  

When we reconnect to the instance, enter following commands:
```
ssh -L 5901:localhost:5901 -i "new-aws.pem" ubuntu@ec2-3-84-129-33.compute-1.amazonaws.com (according to the command in your EC2 instance page)
vncserver -kill :1
vncserver -geometry 1920x1080
exit
```

The last step is using VNC Viewer software and entering `ec2-52-55-1-223.compute-1.amazonaws.com:1` to VNC Viewer. You can start to use Ubuntu 16.04 Desktop with your password:
```
ec2-52-55-1-223.compute-1.amazonaws.com:1 (according to the command in your EC2 instance page)
Your Password
```

__Tips: If you have any question about configuration, please check [video: How To set a GUI in a Ubuntu AWS EC2 instance](https://www.youtube.com/watch?v=9BAoJ7JZHr0&t=1010s) in youtube.__

# Instructions about reconnecting to AWS
**Remember to press "Stop" button in the Instance State when you do not use it. If you do not stop your instance, you will be charged a large amount of money.**

**If you want to run your instance, press "Start" button in the Instance State. Remeber instance's public DNS(IPv4) will change when you stop and restart it, so you have to check some commands in your EC2 instance page when you want to reconnect to the instance.**

If you want to reconnect to the Ubuntu 16.04 Desktop, please open Terminal or Git Bash first and enter:
```
cd Downloads (location where you save your .pem file)
ssh -L 5901:localhost:5901 -i "new-aws.pem" ubuntu@ec2-52-55-1-223.compute-1.amazonaws.com (according the command in your EC2 instance page)
vncserver
```
Open VNC Viewer and enter following commands to reconnect to the Ubuntu 16.04 Desktop. Then you can start testing UB-ANC projects. Follow the instructions at [UB-ANC-Agent](https://github.com/jmodares/UB-ANC-Agent).
```
ec2-52-55-1-223.compute-1.amazonaws.com:1 (according to the command in your EC2 instance page)
```

**Tips: If you fail to reconnect to your instance and Terminal/Git Bash output "Bad Permission" like:**
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@         WARNING: UNPROTECTED PRIVATE KEY FILE!          @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Permissions 0644 for 'youramazon.pem' are too open.
It is recommended that your private key files are NOT accessible by others.
This private key will be ignored.
bad permissions: ignore key: amazonec2.pem
Permission denied (publickey).
```

**You can open Terminal or Git Bash and enter `chmod 400 /Users/myself/Documents/MyAccessKey1.pem`. Then you can try to connect instance again. For more information, please check [How to Fix Permission Error When SSH into Amazon EC2 Instance](https://99robots.com/how-to-fix-permission-error-ssh-amazon-ec2-instance/).**
