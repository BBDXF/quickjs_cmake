import {fetch} from "./fetch.js";

async function t1() {
    try {
        let response = await fetch("https://jsonplaceholder.typicode.com/posts", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ title: 'foo', body: 'bar', userId: 1 })
        });

        console.log("Status:", response.status);
        console.log("OK:", response.ok);
        console.log("Headers:", response.headers);
        response.headers.forEach((value, key) => {
            console.log("  > ", key," => ", value);
          });
        console.log("Text:", await response.text());
        var obj = await response.json();
        console.log("JSON: (title, id) =>", obj.title, obj.id);
    } catch (error) {
        console.log("Error:", error);
    }
}

t1();
// t1().then(_=> console.log("Done"));
// console.log("xxx");