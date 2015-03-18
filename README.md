# EasyDDB direct link

Redirect sub-domains to user profile pages.

## Configuration

* Setup the virtual host of the instance with respective ServerAlias directives. Most common would be 
```ServerAlias *.some-domain.com```
* Uncomment the $cookie_domain variable in Drupal's settings.php file and assign respective setting. Most common would be 
``` $cookie_domain = .some-domain.com.```
This setting will cover any sub-domain derived from "some-domain.com" name.

### Module configuration

Most straight-forward part. Go to ```admin/config/people/easyddb_direct```.
Assign correct settings and save. Default values, when enabling the module,
point to some default user pages in the system.
