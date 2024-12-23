# ConnectOpen
A lightweight tool that makes periodic HTTP requests to simulate connection activates and prevent Anyconnect/Openconnect VPN from idling. The received data will be discarded to save the hard drive.

## Usage

```ConnectOpen [url]```

- **[url]**: The HTTP URL to get from. If not provided, it reaches to `http://onlinefiletools.com/generate-random-text-file?length=4096kb`

## crontab

Add a crontab entry to run the program recurrently 

```* * * * * /path/to/ConnectOpen```

## TODO

- Check HTTPS
- Test the connections