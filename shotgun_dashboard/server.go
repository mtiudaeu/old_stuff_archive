package main

import (
 "io"
 "os"
 "net/http"
 "strings"
 "fmt"
)

func main() {
 http.Handle("/", http.FileServer(http.Dir("public")))

 http.HandleFunc("/api/components", api_handle)
 http.HandleFunc("/api/expand/tk-config-flame/v5.8", api_handle)
 http.HandleFunc("/api/expand/tk-config-flame/v6.0", api_handle)
 http.HandleFunc("/api/expand/tk-config-flame/v6.1.0#3ffba06ff2", api_handle)
 http.HandleFunc("/api/expand/tk-config-flameplugin/v1.0.0", api_handle)
 http.HandleFunc("/api/expand/tk-config-flameplugin/v1.1.0", api_handle)
 http.HandleFunc("/api/expand/tk-config-flameplugin/v2.0.0", api_handle)

 http.ListenAndServe(":8080", nil)
}

func api_handle(w http.ResponseWriter, r *http.Request) {
  filename := r.URL.Path[1:]
  filename = strings.Replace(filename, "/", "_", -1)
  fmt.Println(filename)
  api_send_file_content(w, filename + ".data")
}

func api_send_file_content(w http.ResponseWriter, filename string) {
  if ( w == nil ) {
    return
  }
  w.Header().Set("Access-Control-Allow-Origin", "*")
  DataFile, err := os.Open(filename)
  defer DataFile.Close()
  if err != nil {
    // File not found, send 404
    http.Error(w, "Invalid Request", 404)
    return
  }
  io.Copy(w, DataFile)
}
