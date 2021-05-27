cmd_Release/API.node := ln -f "Release/obj.target/API.node" "Release/API.node" 2>/dev/null || (rm -rf "Release/API.node" && cp -af "Release/obj.target/API.node" "Release/API.node")
