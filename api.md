## Standard structure

### GET / POST

#### Headers:
- [iff POST] Content-Type: application/json
- Authorization: Basic *&lt;base64 encoded api key>*

#### Body:
```JSON
{
	"data": <endpoint specific>,
	"success": <true|false>,
	"error": "<present iff success is false>"
}
```

#### Status code:
- 200: OK
- 400: Error parsing JSON
- 422: Invalid request e.g. option doesn't exist
- 404: Resource doesn't exist
- 401: Malformed or no api key provided
- 403: Invalid api key
- 500: Server error

#### Response headers:
- [iff status code 401] WWW-Authenticate: Basic


## Endpoints

### GET /
This document

### GET /method_mirror
Mirrors requests with the specified method to the specified endpoint (for clients with limited http capabilities).

#### Url params:
- method: method, defaults to POST
- path: path of endpoint
- data: post data, url encoded

#### Example:
- method: POST (omitted)
- path: /rgb_strip
- data: {"mode":"color","color":"ffffff"}

Resulting request:  
GET /method_mirror?path=%2Frgb_strip&data=%7B%22mode%22%3A%22color%22%2C%22color%22%3A%22ffffff%22%7D

### GET /rgb_strip
```JSON
{
	"mode": "<color|weather|effect>",
	"effect": "<rainbow|breathing|rolling_rainbow>",
	"speed": "<0-100>",
	"color": "<strip color>"
}
```

### POST /rgb_strip
Any combination of the above properties.

### GET /switches
```JSON
[
	{
		"id": "<sequential int e.g. 0, 4, 9>",
		"name": "<friendly name>",
		"state": "<on|off>"
	}

]
```

### GET /switches/&lt;switch id>
Idem to /switches item.

### POST /switches/&lt;switch id>
```JSON
{
	"state": "<on|off|toggle>"
}
```