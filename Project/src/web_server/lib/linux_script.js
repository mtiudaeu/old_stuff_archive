function tmp() {
   console.log('tmp');
};

exports.tmp = tmp;

function grepRequest(req) {
   var ret;
   if(req.charAt(0)==='f') {
      ret = 'TMP grepRequest';
      
   }
   return ret;
}

function editRequest(req) {
   var ret;
   if(req.charAt(0)==='f') {
//MDTMP include argument
      ret = 'sf archive.C | xargs vim -p ';
   }
   return ret;
}

function request(req) {
   var ret;

   if(req.length<3) {
      //MDTMP error
      return ret;
   }

   if(req.charAt(0)==='s') {
      if(req.charAt(1)==='g') {
         ret = grepRequest(req.substr(2,req.length-1));
      }
      else if(req.charAt(1)==='e') {
         ret = editRequest(req.substr(2,req.length-1));
      }
   }

   return ret;
};
exports.request = request;
