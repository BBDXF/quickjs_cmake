import * as std from "std";

// 使用curl命令行工具发起HTTP请求，并解析响应。
function fetch(url, options) {
    let method = "GET";
    let headers = "";
    let body = "";

    if (options) {
        if (options.method) {
            method = options.method;
        }
        if (options.headers) {
            for (let key in options.headers) {
                if (options.headers.hasOwnProperty(key)) {
                    headers += `-H '${key}: ${options.headers[key]}' `;
                }
            }
        }
        if (options.body) {
            body = options.body;
        }
    }

    let command = `curl -s -i -X ${method} "${url}" ${headers} -d '${body}'`;

    let popen = std.popen(command, "r");
    // console.log("curl f:"+popen+", cmd: "+command);

    if (!popen) {
        throw new Error("Failed to open process");
    }

    let responseText = popen.readAsString();
    popen.close();

    let [headerText, responseBody] = responseText.split("\r\n\r\n");
    if (!responseBody) {
        [headerText, responseBody] = responseText.split("\n\n");
    }

    let headersObj = new Map();
    let lines = headerText.split("\n");
    for (let i = 1; i < lines.length; i++) {
        let [key, ...valueParts] = lines[i].split(": ");
        if (key && valueParts.length > 0) {
            headersObj.set(key, valueParts.join(": "));
        }
    }

    let statusLine = lines[0].trim();
    let statusParts = statusLine.split(" ");
    let statusCode = parseInt(statusParts[1], 10);

    return {
        ok: statusCode >= 200 && statusCode < 300,
        status: statusCode,
        statusText: statusParts.slice(2).join(" "),
        headers: headersObj,
        text: function() {
            return responseBody.trim();
        },
        json: async function() {
            try {
                return JSON.parse(responseBody.trim());
            } catch (e) {
                throw new Error("Failed to parse JSON response");
            }
        }
    };
}

export {
    fetch
};
