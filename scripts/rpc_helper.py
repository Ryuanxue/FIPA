from pycparser import c_ast, c_generator

basic_type=['int','char','unsigned char','float','double','void','long','short','signed','unsigned','bool'] #基本类型
typedef_dict={} #存放typedef的定义
struct_dict={} #存放struct的定义

space4='    ' #缩进4个空格
space8='        ' #缩进8个空格
space12='            ' #缩进12个空格
space16='                ' #缩进16个空格

generator= c_generator.CGenerator()

def recur_dealwith_struct_ptr_server_param(structast,leftname,righname,opname,typename,declname,unmashal_str_list,processed_types=None):
    # 如果没有传递processed_types，则创建新的集合
    if processed_types is None:
        processed_types = set()
        
    unmashal_str_list.append("if ("+righname+opname+declname+opname+typename+"_rpc_ptr_val == NULL) {"+leftname+opname+declname+"=NULL;}")
    if typename in typedef_dict:
        unmashal_str_list.append("else {"+leftname+opname+declname+"=("+typename+" *)malloc("+righname+opname+declname+opname+typename+"_rpc_ptr_len * sizeof("+typename+"));")
    else:
        unmashal_str_list.append("else {"+leftname+opname+declname+"=(struct "+typename+" *)malloc("+righname+opname+declname+opname+typename+"_rpc_ptr_len * sizeof(struct "+typename+"));")
    unmashal_str_list.append("\tif ("+leftname+opname+declname+" == NULL) {")
    unmashal_str_list.append("\t\tperror(\"malloc failed\");")
    unmashal_str_list.append("\t\texit(1);")
    unmashal_str_list.append("\t}")
    unmashal_str_list.append("\tfor (int i1 = 0; i1 < "+righname+opname+declname+opname+typename+"_rpc_ptr_len; i1++) {")
    recur_dealwith_struct_server_param(structast, leftname+opname+declname+"[i1]", righname+opname+declname+opname+typename+"_rpc_ptr_val[i1]", ".",unmashal_str_list,processed_types)
    unmashal_str_list.append("\t}")
    unmashal_str_list.append("}")
    
def recur_dealwith_struct_server_param(structast,leftname,righname,opname,unmashal_str_list,processed_types=None):
    # 初始化已处理类型集合
    if processed_types is None:
        processed_types = set()
    
    # 获取结构体名称用于循环检测
    if isinstance(structast, c_ast.Typedef):
        struct_name = structast.name
        structast = structast.type.type
    elif hasattr(structast, 'name') and structast.name:
        struct_name = structast.name
    else:
        struct_name = str(id(structast))  # 没有名称时使用对象ID
    
    # 检查是否已处理过该类型，避免循环
    if struct_name in processed_types:
        return
    
    processed_types.add(struct_name)
    
    if structast.decls is None:
        if structast.name in struct_dict:
            structast=struct_dict[structast.name]['ast']
        else:
            return
    for decl in structast.decls:
        decltype=decl.type
        decltypename=generator.visit(decltype)
        if isinstance(decltype,c_ast.TypeDecl):
            decltypename=decltypename.split(" ")[-1]
            if decltypename in basic_type:
                unmashal_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
            elif decltypename=="stat":
                recur_dealwith_stat_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list)
            elif decltypename in typedef_dict:
                if typedef_dict[decltypename]['isenum']:
                    unmashal_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                elif typedef_dict[decltypename]['isstruct']:
                    recur_dealwith_struct_server_param(typedef_dict[decltypename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list,processed_types)
                else:
                    if isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.Union):
                        continue
                    if isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.FuncDecl):
                        continue
                    if not isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.IdentifierType):
                        continue
                    typename=typedef_dict[decltypename]['ast'].type.type.names[0]
                    if typename in basic_type:
                        unmashal_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                    elif typename in struct_dict:
                        if typename=="stat":
                            recur_dealwith_stat_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list)
                        elif struct_dict[typename]['isenum']:
                            unmashal_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                        elif struct_dict[typename]['isstruct']:
                            recur_dealwith_struct_server_param(struct_dict[typename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list,processed_types)
                        else:
                            pass
            elif decltypename in struct_dict:
                if decltypename=="stat":
                    recur_dealwith_stat_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list)
                elif struct_dict[decltypename]['isenum']:
                    unmashal_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                elif struct_dict[decltypename]['isstruct']:
                    recur_dealwith_struct_server_param(struct_dict[decltypename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list,processed_types)
                else:
                    pass
        

        elif isinstance(decltype,c_ast.PtrDecl):
            if decltypename=="char *" or decltypename=="const char *" or decltypename=="FILE *":
                unmashal_str_list.append("if ("+righname+opname+decl.name+".charptr.charptr_val == NULL) {"+leftname+opname+decl.name+"=NULL;}\n")
                unmashal_str_list.append("else {"+leftname+opname+decl.name+"=malloc("+righname+opname+decl.name+".charptr.charptr_len);\n")
                unmashal_str_list.append("memcpy("+leftname+opname+decl.name+","+righname+opname+decl.name+".charptr.charptr_val,"+righname+opname+decl.name+".charptr.charptr_len);\n")
                unmashal_str_list.append("}\n")
            elif decltypename=="void *":
                unmashal_str_list.append("if ("+righname+opname+decl.name+opname+decl.name+"_val == NULL) {"+leftname+opname+decl.name+"=NULL;}\n")
                unmashal_str_list.append("else {"+leftname+opname+decl.name+"=malloc("+righname+opname+decl.name+opname+decl.name+"_len);\n")
                unmashal_str_list.append("\tif ("+leftname+opname+decl.name+" == NULL) {\n")
                unmashal_str_list.append("\t\tperror(\"malloc failed\");\n")
                unmashal_str_list.append("\t\texit(1);\n")
                unmashal_str_list.append("\t}\n")
                unmashal_str_list.append("\tmemcpy("+leftname+opname+decl.name+","+righname+opname+decl.name+opname+decl.name+"_val,"+righname+opname+decl.name+opname+decl.name+"_len);\n")
                unmashal_str_list.append("}\n")
            

            elif decltypename=="char **" or decltypename=="const char **":
                unmashal_str_list.append("if ("+righname+opname+decl.name+".twocharptr.twocharptr_val == NULL) {"+leftname+opname+decl.name+"=NULL;}\n")
                unmashal_str_list.append("else {"+leftname+opname+decl.name+"=(char **)malloc("+righname+opname+decl.name+".twocharptr.twocharptr_len * sizeof(char *));\n")
                unmashal_str_list.append("\tif ("+leftname+opname+decl.name+" == NULL) {\n")
                unmashal_str_list.append("\t\tperror(\"malloc failed\");\n")
                unmashal_str_list.append("\t\texit(1);\n")
                unmashal_str_list.append("\t}\n")
                unmashal_str_list.append("\tfor (int i = 0; i < "+righname+opname+decl.name+".twocharptr.twocharptr_len; i++) {\n")
                unmashal_str_list.append("\t\tif ("+righname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {\n")
                unmashal_str_list.append("\t\t\t"+leftname+opname+decl.name+"[i] = NULL;\n")
                unmashal_str_list.append("\t\t} else {\n")
                unmashal_str_list.append("\t\t\t"+leftname+opname+decl.name+"[i] = strdup("+righname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val);\n")
                unmashal_str_list.append("\t\t}\n")
                unmashal_str_list.append("\t}\n")
                unmashal_str_list.append("}\n")
                
            elif decltypename=="struct stat *":
                pass
            else:
                #二级指针跳过,FuncDecl跳过,Union跳过
                if isinstance(decltype.type,c_ast.Union):
                    continue
                if isinstance(decltype.type,c_ast.FuncDecl):
                    continue
                if (generator.visit(decltype.type)=="void *"):
                    continue
                if isinstance(decltype.type,c_ast.PtrDecl):
                    continue
                if isinstance(decltype.type.type,c_ast.Struct):
                    typename=decltype.type.type.name
                else:
                    typename=decltype.type.type.names[0]
                if typename in basic_type:
                    recur_dealwith_basicptr_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,unmashal_str_list)

                elif typename in typedef_dict:
                    if typedef_dict[typename]['isenum']:
                        recur_dealwith_basicptr_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,unmashal_str_list)
                    elif typedef_dict[typename]['isstruct']:
                        recur_dealwith_struct_ptr_server_param(typedef_dict[typename]['ast'],leftname,righname,opname,typename,decl.name,unmashal_str_list,processed_types)
                    else:
                        if not isinstance(typedef_dict[typename]['ast'].type.type,c_ast.IdentifierType):
                            continue
                        typename=typedef_dict[typename]['ast'].type.type.names[0]
                        if typename in basic_type:
                            recur_dealwith_basicptr_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,unmashal_str_list)
                        elif typename in struct_dict:
                            if typename=="stat":
                                pass
                            elif struct_dict[typename]['isenum']:
                                recur_dealwith_basicptr_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,unmashal_str_list)
                            elif struct_dict[typename]['isstruct']:
                                recur_dealwith_struct_ptr_server_param(struct_dict[typename]['ast'],leftname,righname,opname,typename,decl.name,unmashal_str_list,processed_types)
                            else:
                                pass
                elif typename in struct_dict:
                    if typename=="stat":
                        pass
                    elif struct_dict[typename]['isenum']:
                        recur_dealwith_basicptr_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,unmashal_str_list)
                    elif struct_dict[typename]['isstruct']:
                        recur_dealwith_struct_ptr_server_param(struct_dict[typename]['ast'],leftname,righname,opname,typename,decl.name,unmashal_str_list,processed_types)
                    else:
                        pass

def recur_dealwith_struct_client_retvalue(structast,leftname,righname,opname,unmashal_str_list,wrapper_str_list):
    if isinstance(structast,c_ast.Typedef):
        structast=structast.type.type
    if structast.decls is None:
        if structast.name in struct_dict:
            structast=struct_dict[structast.name]['ast']
        else:
            return
    for decl in structast.decls:
        decltype=decl.type
        decltypename=generator.visit(decltype)
        if isinstance(decltype,c_ast.TypeDecl):
            decltypename=decltypename.split(" ")[-1]
            if decltypename in basic_type:
                unmashal_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
            elif decltypename=="stat":
                recur_dealwith_stat_client_param(leftname+opname+decl.name,righname+opname+decl.name,".",wrapper_str_list)
            elif decltypename in typedef_dict:
                if typedef_dict[decltypename]['isenum']:
                    wrapper_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                elif typedef_dict[decltypename]['isstruct']:
                    recur_dealwith_struct_server_param(typedef_dict[decltypename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list,None)
                else:
                    if isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.Union):
                        continue
                    if isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.FuncDecl):
                        continue
                    typename=typedef_dict[decltypename]['ast'].type.type.names[0]
                    if typename in basic_type:
                        unmashal_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                    elif typename in struct_dict:
                        if typename=="stat":
                            recur_dealwith_stat_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list)
                        elif struct_dict[typename]['isenum']:
                            unmashal_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                        elif struct_dict[typename]['isstruct']:
                            recur_dealwith_struct_server_param(struct_dict[typename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list,None)
                        else:
                            pass
            elif decltypename in struct_dict:
                if decltypename=="stat":
                    recur_dealwith_stat_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list)
                elif struct_dict[decltypename]['isenum']:
                    unmashal_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                elif struct_dict[decltypename]['isstruct']:
                    recur_dealwith_struct_server_param(struct_dict[decltypename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",unmashal_str_list,None)
                else:
                    pass
        

        elif isinstance(decltype,c_ast.PtrDecl):
            if decltypename=="char *" or decltypename=="const char *" or decltypename=="FILE *":
                unmashal_str_list.append("if ("+righname+opname+decl.name+".charptr.charptr_val == NULL) {"+leftname+opname+decl.name+"=NULL;}")
                unmashal_str_list.append("else {"+leftname+opname+decl.name+"=malloc("+righname+opname+decl.name+".charptr.charptr_len);")
                unmashal_str_list.append("memcpy("+leftname+opname+decl.name+","+righname+opname+decl.name+".charptr.charptr_val,"+righname+opname+decl.name+".charptr.charptr_len);")
                unmashal_str_list.append("}")
            elif decltypename=="void *":
                pass
            elif decltypename=="char **" or decltypename=="const char **":
                unmashal_str_list.append("if ("+righname+opname+decl.name+".twocharptr.twocharptr_val == NULL) {"+leftname+opname+decl.name+"=NULL;}")
                unmashal_str_list.append("else {"+leftname+opname+decl.name+"=(char **)malloc("+righname+opname+decl.name+".twocharptr.twocharptr_len * sizeof(char *));")
                unmashal_str_list.append("\tif ("+leftname+opname+decl.name+" == NULL) {")
                unmashal_str_list.append("\t\tperror(\"malloc failed\");")
                unmashal_str_list.append("\t\texit(1);")
                unmashal_str_list.append("\t}")
                unmashal_str_list.append("\tfor (int i = 0; i < "+righname+opname+decl.name+".twocharptr.twocharptr_len; i++) {")
                unmashal_str_list.append("\t\tif ("+righname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {")
                unmashal_str_list.append("\t\t\t"+leftname+opname+decl.name+"[i] = NULL;")
                unmashal_str_list.append("\t\t} else {")
                unmashal_str_list.append("\t\t\t"+leftname+opname+decl.name+"[i] = strdup("+righname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val);")
                unmashal_str_list.append("\t\t}")
                unmashal_str_list.append("\t}")
                unmashal_str_list.append("}")
                
            elif decltypename=="struct stat *":
                pass
            else:
                #二级指针跳过,FuncDecl跳过,Union跳过
                if isinstance(decltype.type,c_ast.Union):
                    continue
                if isinstance(decltype.type,c_ast.FuncDecl):
                    continue
                if (generator.visit(decltype.type)=="void *"):
                    continue
                if isinstance(decltype.type,c_ast.PtrDecl):
                    continue
                if isinstance(decltype.type.type,c_ast.Struct):
                    typename=decltype.type.type.name
                else:
                    typename=decltype.type.type.names[0]
                if typename in basic_type:
                    recur_dealwith_basicptr_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,unmashal_str_list)

                elif typename in typedef_dict:
                    if typedef_dict[typename]['isenum']:
                        recur_dealwith_basicptr_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,unmashal_str_list)
                    elif typedef_dict[typename]['isstruct']:
                        recur_dealwith_struct_ptr_server_param(typedef_dict[typename]['ast'],leftname,righname,opname,typename,decl.name,unmashal_str_list,processed_types)
                    else:
                        typename=typedef_dict[typename]['ast'].type.type.names[0]
                        if typename in basic_type:
                            recur_dealwith_basicptr_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,unmashal_str_list)
                        elif typename in struct_dict:
                            if typename=="stat":
                                pass
                            elif struct_dict[typename]['isenum']:
                                recur_dealwith_basicptr_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,unmashal_str_list)
                            elif struct_dict[typename]['isstruct']:
                                recur_dealwith_struct_ptr_server_param(struct_dict[typename]['ast'],leftname,righname,opname,typename,decl.name,unmashal_str_list,processed_types)
                            else:
                                pass
                elif typename in struct_dict:
                    if typename=="stat":
                        pass
                    elif struct_dict[typename]['isenum']:
                        recur_dealwith_basicptr_server_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,unmashal_str_list)
                    elif struct_dict[typename]['isstruct']:
                        recur_dealwith_basicptr_server_param(leftname,righname,opname,typename,unmashal_str_list)
                    else:
                        pass


def dealwith_struct_client_server_param(typename,paramname,paramcount,wrapper_str_list,clnt_rpc_call_param_list,svc_param_list,unmashal_str_list):
    # 客户端封装数据
    wrapper_str_list.append(typename+"_rpc "+paramname+"_rpc;")
    clnt_rpc_call_param_list.append(paramname+"_rpc")
    if typename in typedef_dict:
        recur_dealwith_struct_client_param(typedef_dict[typename]['ast'],paramname+"_rpc",paramname,".",wrapper_str_list,0,None)
    else:
        recur_dealwith_struct_client_param(struct_dict[typename]['ast'],paramname+"_rpc",paramname,".",wrapper_str_list,0,None)
    #服务端解封装数据
    svc_param_list.append(typename+"_rpc arg"+str(paramcount))
    if typename in typedef_dict:
        unmashal_str_list.append(typename+" "+paramname+";")
    else:
        unmashal_str_list.append("struct "+typename+" "+paramname+";")
    if typename in typedef_dict:
        recur_dealwith_struct_server_param(typedef_dict[typename]['ast'],paramname,"arg"+str(paramcount),".",unmashal_str_list,None)
    else:
        recur_dealwith_struct_server_param(struct_dict[typename]['ast'],paramname,"arg"+str(paramcount),".",unmashal_str_list,None)

def dealwith_struct_client_server_retvalue(typename,paramname,client_rpc_ret_str_list,server_rpc_ret_str_list):
    # 客户端解封装数据
    if typename in typedef_dict:
        recur_dealwith_struct_server_param(typedef_dict[typename]['ast'],"default_value","result_rpc->value",".",server_rpc_ret_str_list,None)
    else:
        recur_dealwith_struct_server_param(struct_dict[typename]['ast'],"default_value","result_rpc->value",".",server_rpc_ret_str_list,None)
    #服务端解封装数据
    # svc_param_list.append(typename+"_rpc arg"+str(paramcount))
    # if typename in typedef_dict:
    #     unmashal_str_list.append(typename+" "+paramname+";")
    # else:
    #     unmashal_str_list.append("struct "+typename+" "+paramname+";")
    # if typename in typedef_dict:
    #     recur_dealwith_struct_server_param(typedef_dict[typename]['ast'],paramname,"arg"+str(paramcount),".",unmashal_str_list,wrapper_str_list)
    # else:
    #     recur_dealwith_struct_server_param(struct_dict[typename]['ast'],paramname,"arg"+str(paramcount),".",unmashal_str_list,wrapper_str_list)

    

def dealwith_structptr_client_param(typename,paramname, wrapper_str_list, clnt_rpc_call_param_list, depth=0):
    # 客户端封装数据
    wrapper_str_list.append(typename+"_rpc_ptr "+paramname+"_rpc_ptr;")
    clnt_rpc_call_param_list.append(paramname+"_rpc_ptr")
    wrapper_str_list.append("if("+paramname+"==NULL) {")
    wrapper_str_list.append(paramname+"_rpc_ptr."+typename+"_rpc_ptr_len=0;")
    wrapper_str_list.append(paramname+"_rpc_ptr."+typename+"_rpc_ptr_val=NULL;")
    wrapper_str_list.append("} else {")
    wrapper_str_list.append(paramname+"_rpc_ptr."+typename+"_rpc_ptr_len=1;")
    wrapper_str_list.append(paramname+"_rpc_ptr."+typename+"_rpc_ptr_val=malloc(1*sizeof("+typename+"_rpc));")
    wrapper_str_list.append("if("+paramname+"_rpc_ptr."+typename+"_rpc_ptr_val==NULL) {")
    wrapper_str_list.append("perror(\"malloc failed\");")
    wrapper_str_list.append("exit(1);")
    wrapper_str_list.append("}")
    # wrapper_str_list.append(f"for(int i=0;i<"+paramname+"_rpc_ptr."+typename+"_rpc_ptr_len;i++) {")
    wrapper_str_list.append(f"for(int i{depth}=0;i{depth}<{paramname}_rpc_ptr.{typename}_rpc_ptr_len;i{depth}++) {{")

    #检查是否typename在typedef_dict中
    if typename in typedef_dict:
        recur_dealwith_struct_client_param(typedef_dict[typename]['ast'],paramname+"_rpc_ptr."+typename+"_rpc_ptr_val[i0]",paramname+"[i0]",".",wrapper_str_list,0,None)
    else:
        recur_dealwith_struct_client_param(struct_dict[typename]['ast'],paramname+"_rpc_ptr."+typename+"_rpc_ptr_val[i0]",paramname+"[i0]",".",wrapper_str_list,0,None)
    wrapper_str_list.append("}")
    wrapper_str_list.append("}")

def dealwith_structptr_server_param(typename,paramname,paramcount,svc_param_list,unmashal_str_list):
    #服务端解封装数据
    svc_param_list.append(typename+"_rpc_ptr arg"+str(paramcount))
    if typename in typedef_dict:
        unmashal_str_list.append(typename+" *"+paramname+"=NULL;")
    else:
        unmashal_str_list.append("struct "+typename+" *"+paramname+"=NULL;")
    unmashal_str_list.append("if(arg"+str(paramcount)+"."+typename+"_rpc_ptr_val!=NULL) {")
    if typename in typedef_dict:
        unmashal_str_list.append(paramname+"=( "+typename+" *)malloc(arg"+str(paramcount)+"."+typename+"_rpc_ptr_len*sizeof( "+typename+" *));")
    else:
        unmashal_str_list.append(paramname+"=(struct "+typename+" *)malloc(arg"+str(paramcount)+"."+typename+"_rpc_ptr_len*sizeof(struct "+typename+"));")
    unmashal_str_list.append("if("+paramname+"==NULL) {")
    unmashal_str_list.append("perror(\"malloc failed\");")
    unmashal_str_list.append("exit(1);")
    unmashal_str_list.append("}")
    unmashal_str_list.append("for(int i0=0;i0<arg"+str(paramcount)+"."+typename+"_rpc_ptr_len;i0++) {")
    #检查是否typename在typedef_dict中
    if typename in typedef_dict:
        recur_dealwith_struct_server_param(typedef_dict[typename]['ast'],paramname+"[i0]","arg"+str(paramcount)+"."+typename+"_rpc_ptr_val[i0]",".",unmashal_str_list,None)
    else:
        recur_dealwith_struct_server_param(struct_dict[typename]['ast'],paramname+"[i0]","arg"+str(paramcount)+"."+typename+"_rpc_ptr_val[i0]",".",unmashal_str_list,None)
    unmashal_str_list.append("\t}")
    unmashal_str_list.append("}")

def dealwith_struct_server_param_global_basic(typename,paramname,unmashal_str_list):
    #服务端解封装数据
    #检查是否typename在typedef_dict中
    
    if typename in typedef_dict:
        unmashal_str_list.append(f"{typename} {paramname};")
        recur_dealwith_struct_server_param(typedef_dict[typename]['ast'],paramname,"value",".",unmashal_str_list,None)
    else:
        unmashal_str_list.append(f"struct {typename} {paramname};")
        recur_dealwith_struct_server_param(struct_dict[typename]['ast'],paramname,"value",".",unmashal_str_list,None)

def dealwith_structptr_client_paramRet(typename,paramname, client_rpc_ret_str_list):
    client_rpc_ret_str_list.append(space4+"if(result_rpc->"+paramname+"."+typename+"_rpc_ptr_val==NULL) {")
    client_rpc_ret_str_list.append(space8+paramname+"=NULL;")
    client_rpc_ret_str_list.append(space4+"} else {")
    client_rpc_ret_str_list.append(space8+paramname+"=malloc(result_rpc->"+paramname+"."+typename+"_rpc_ptr_len*sizeof(struct "+typename+"));") 
    client_rpc_ret_str_list.append(space8+"if("+paramname+"==NULL) {")
    client_rpc_ret_str_list.append(space12+"perror(\"malloc failed\");")
    client_rpc_ret_str_list.append(space12+"exit(1);")
    client_rpc_ret_str_list.append(space8+"}")
    client_rpc_ret_str_list.append(space8+"for(int i0=0;i0<result_rpc->"+paramname+"."+typename+"_rpc_ptr_len;i0++) {")
    if typename in typedef_dict:
        recur_dealwith_struct_client_retvalue(typedef_dict[typename]['ast'],paramname+"[i0]","result_rpc->"+paramname+"."+typename+"_rpc_ptr_val[i0]",".",client_rpc_ret_str_list,None,1)
    else:
        recur_dealwith_struct_client_retvalue(struct_dict[typename]['ast'],paramname+"[i0]","result_rpc->"+paramname+"."+typename+"_rpc_ptr_val[i0]",".",client_rpc_ret_str_list,None,1)
    client_rpc_ret_str_list.append(space8+"}")
    client_rpc_ret_str_list.append(space4+"}")


def dealwith_structptr_server_paramRet(typename,paramname,server_rpc_ret_str_list):
    server_rpc_ret_str_list.append(space4+"if("+paramname+"==NULL) {")
    server_rpc_ret_str_list.append(space8+"result."+paramname+"."+typename+"_rpc_ptr_len=0;")  
    server_rpc_ret_str_list.append(space8+"result."+paramname+"."+typename+"_rpc_ptr_val=NULL;")
    server_rpc_ret_str_list.append(space4+"} else {")
    server_rpc_ret_str_list.append(space8+"result."+paramname+"."+typename+"_rpc_ptr_len=1;")
    server_rpc_ret_str_list.append(space8+"result."+paramname+"."+typename+"_rpc_ptr_val=malloc(1*sizeof("+typename+"_rpc));")
    server_rpc_ret_str_list.append(space8+"if(result."+paramname+"."+typename+"_rpc_ptr_val==NULL) {")
    server_rpc_ret_str_list.append(space12+"perror(\"malloc failed\");")
    server_rpc_ret_str_list.append(space12+"exit(1);")
    server_rpc_ret_str_list.append(space8+"}")
    server_rpc_ret_str_list.append(space8+"for(int i0=0;i0<result."+paramname+"."+typename+"_rpc_ptr_len;i0++) {")
    if typename in typedef_dict:
        recur_dealwith_struct_server_retvalue(typedef_dict[typename]['ast'],"result."+paramname+"."+typename+"_rpc_ptr_val[i0]",paramname+"[i0]",".",server_rpc_ret_str_list,0,None)
    else:
        recur_dealwith_struct_server_retvalue(struct_dict[typename]['ast'],"result."+paramname+"."+typename+"_rpc_ptr_val[i0]",paramname+"[i0]",".",server_rpc_ret_str_list,0,None)
    server_rpc_ret_str_list.append(space8+"}")
    server_rpc_ret_str_list.append(space4+"}")

def dealwith_structptr_server_paramRet_global(typename,paramname,server_rpc_ret_str_list):
    server_rpc_ret_str_list.append(space4+"if("+paramname+"==NULL) {")
    server_rpc_ret_str_list.append(space8+"result.value."+typename+"_rpc_ptr_len=0;")  
    server_rpc_ret_str_list.append(space8+"result.value."+typename+"_rpc_ptr_val=NULL;")
    server_rpc_ret_str_list.append(space4+"} else {")
    server_rpc_ret_str_list.append(space8+"result.value."+typename+"_rpc_ptr_len=1;")
    server_rpc_ret_str_list.append(space8+"result.value."+typename+"_rpc_ptr_val=malloc(1*sizeof("+typename+"_rpc));")
    server_rpc_ret_str_list.append(space8+"if(result.value."+typename+"_rpc_ptr_val==NULL) {")
    server_rpc_ret_str_list.append(space12+"perror(\"malloc failed\");")
    server_rpc_ret_str_list.append(space12+"exit(1);")
    server_rpc_ret_str_list.append(space8+"}")
    server_rpc_ret_str_list.append(space8+"for(int i0=0;i0<result.value."+typename+"_rpc_ptr_len;i0++) {")
    if typename in typedef_dict:
        recur_dealwith_struct_server_retvalue(typedef_dict[typename]['ast'],"result.value."+typename+"_rpc_ptr_val[i0]",paramname+"[i0]",".",server_rpc_ret_str_list,0,None)
    else:
        recur_dealwith_struct_server_retvalue(struct_dict[typename]['ast'],"result.value."+typename+"_rpc_ptr_val[i0]",paramname+"[i0]",".",server_rpc_ret_str_list,0,None)
    server_rpc_ret_str_list.append(space8+"}")
    server_rpc_ret_str_list.append(space4+"}")

def dealwith_struct_server_paramRet_global_basic(typename,paramname,server_rpc_ret_str_list):
    if typename in typedef_dict:
        recur_dealwith_struct_client_param(typedef_dict[typename]['ast'],"result.value",paramname,".",server_rpc_ret_str_list,0,None)
    else:
        recur_dealwith_struct_client_param(struct_dict[typename]['ast'],"result.value",paramname,".",server_rpc_ret_str_list,0,None)


def recur_dealwith_struct_server_retvalue(structast,leftname,righname,opname,server_rpc_ret_str_list,depth=0,processed_types=None):
    # 初始化已处理类型集合
    if processed_types is None:
        processed_types = set()
    
    # 获取结构体名称用于循环检测
    if isinstance(structast, c_ast.Typedef):
        struct_name = structast.name
        structast = structast.type.type
    elif hasattr(structast, 'name') and structast.name:
        struct_name = structast.name
    else:
        struct_name = str(id(structast))  # 没有名称时使用对象ID
    
    # 检查是否已处理过该类型，避免循环
    if struct_name in processed_types:
        return
    
    processed_types.add(struct_name)
    
    if structast.decls is None:
        if structast.name in struct_dict:
            structast=struct_dict[structast.name]['ast']
        else:
            return
    for decl in structast.decls:
        decltype=decl.type
        decltypename=generator.visit(decltype)
        if isinstance(decltype,c_ast.TypeDecl):
            decltypename=decltypename.split(" ")[-1]
            if decltypename in basic_type:
                if depth==1:
                    server_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+"->"+decl.name+";")
                else:
                    server_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
            elif decltypename=="stat":
                if depth==1:
                    recur_dealwith_stat_server_retvalue(leftname+opname+decl.name,righname+"->"+decl.name,".",server_rpc_ret_str_list)
                else:
                    recur_dealwith_stat_server_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",server_rpc_ret_str_list)
            elif decltypename in typedef_dict:
                if typedef_dict[decltypename]['isenum']:
                    if depth==1:
                        server_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+"->"+decl.name+";")
                    else:
                        server_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                elif typedef_dict[decltypename]['isstruct']:
                    if depth==1:
                        recur_dealwith_struct_server_retvalue(typedef_dict[decltypename]['ast'],leftname+opname+decl.name,righname+"->"+decl.name,".",server_rpc_ret_str_list,depth,processed_types)
                    else:
                        recur_dealwith_struct_server_retvalue(typedef_dict[decltypename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",server_rpc_ret_str_list,depth,processed_types)
                else:
                    if isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.Union):
                        continue
                    if isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.FuncDecl):
                        continue
                    if not isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.IdentifierType):
                        continue
                    typename=typedef_dict[decltypename]['ast'].type.type.names[0]
                    if typename in basic_type:
                        if depth==1:
                            server_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+"->"+decl.name+";")
                        else:
                            server_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                    elif typename in struct_dict:
                        if typename=="stat":
                            if depth==1:
                                recur_dealwith_stat_server_retvalue(leftname+opname+decl.name,righname+"->"+decl.name,".",server_rpc_ret_str_list)
                            else:
                                recur_dealwith_stat_server_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",server_rpc_ret_str_list)
                        elif struct_dict[typename]['isenum']:
                            if depth==1:
                                server_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+"->"+decl.name+";")
                            else:
                                server_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                        elif struct_dict[typename]['isstruct']:
                            if depth==1:
                                recur_dealwith_struct_server_retvalue(struct_dict[typename]['ast'],leftname+opname+decl.name,righname+"->"+decl.name,".",server_rpc_ret_str_list,depth,processed_types)
                            else:
                                recur_dealwith_struct_server_retvalue(struct_dict[typename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",server_rpc_ret_str_list,depth,processed_types)
                        else:
                            pass
            elif decltypename in struct_dict:
                if decltypename=="stat":
                    if depth==1:
                        recur_dealwith_stat_server_retvalue(leftname+opname+decl.name,righname+"->"+decl.name,".",server_rpc_ret_str_list)
                    else:
                        recur_dealwith_stat_server_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",server_rpc_ret_str_list)
                elif struct_dict[decltypename]['isenum']:
                    if depth==1:
                        server_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+"->"+decl.name+";")
                    else:
                        server_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                elif struct_dict[decltypename]['isstruct']:
                    if depth==1:
                        recur_dealwith_struct_server_retvalue(struct_dict[decltypename]['ast'],leftname+opname+decl.name,righname+"->"+decl.name,".",server_rpc_ret_str_list,depth,processed_types)
                    else:
                        recur_dealwith_struct_server_retvalue(struct_dict[decltypename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",server_rpc_ret_str_list,depth,processed_types)
                else:
                    pass
        

        elif isinstance(decltype,c_ast.PtrDecl):
            if decltypename=="char *" or decltypename=="const char *" or decltypename=="FILE *" or decltypename=="u_char *":
                if depth==1:
                    server_rpc_ret_str_list.append("if ("+righname+"->"+decl.name+" == NULL) {"+leftname+opname+decl.name+".charptr.charptr_len = 0;"+leftname+opname+decl.name+".charptr.charptr_val = NULL;}\n")
                    server_rpc_ret_str_list.append("else {"+leftname+opname+decl.name+".charptr.charptr_len = strlen("+righname+"->"+decl.name+")+1;"+leftname+opname+decl.name+".charptr.charptr_val = "+righname+"->"+decl.name+";}\n")
                else:
                    server_rpc_ret_str_list.append("if ("+righname+opname+decl.name+" == NULL) {"+leftname+opname+decl.name+".charptr.charptr_len = 0;"+leftname+opname+decl.name+".charptr.charptr_val = NULL;}\n")
                    server_rpc_ret_str_list.append("else {"+leftname+opname+decl.name+".charptr.charptr_len = strlen("+righname+opname+decl.name+")+1;"+leftname+opname+decl.name+".charptr.charptr_val = "+righname+opname+decl.name+";}\n")
            elif decltypename=="void *":
                if depth==1:
                    server_rpc_ret_str_list.append("if ("+righname+"->"+decl.name+" == NULL) {"+leftname+opname+decl.name+opname+decl.name+"_len = 0;"+leftname+opname+decl.name+opname+decl.name+"_val = NULL;}\n")
                    server_rpc_ret_str_list.append("else{"+leftname+opname+decl.name+opname+decl.name+"_len = sizeof("+righname+"->"+decl.name+");"+leftname+opname+decl.name+opname+decl.name+"_val = malloc("+leftname+opname+decl.name+opname+decl.name+"_len);\n")
                    server_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+opname+decl.name+"_val == NULL) {perror(\"malloc failed\");exit(1);}")
                    server_rpc_ret_str_list.append("\tmemcpy("+leftname+opname+decl.name+opname+decl.name+"_val, "+righname+"->"+decl.name+", "+leftname+opname+decl.name+opname+decl.name+"_len);}\n")
                else:
                    server_rpc_ret_str_list.append("if ("+righname+opname+decl.name+" == NULL) {"+leftname+opname+decl.name+opname+decl.name+"_len = 0;"+leftname+opname+decl.name+opname+decl.name+"_val = NULL;}\n")
                    server_rpc_ret_str_list.append("else{"+leftname+opname+decl.name+opname+decl.name+"_len = sizeof("+righname+opname+decl.name+");"+leftname+opname+decl.name+opname+decl.name+"_val = malloc("+leftname+opname+decl.name+opname+decl.name+"_len);\n")
                    server_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+opname+decl.name+"_val == NULL) {perror(\"malloc failed\");exit(1);}")
                    server_rpc_ret_str_list.append("\tmemcpy("+leftname+opname+decl.name+opname+decl.name+"_val, "+righname+opname+decl.name+", "+leftname+opname+decl.name+opname+decl.name+"_len);}\n")

            elif decltypename=="char **" or decltypename=="const char **":
                if depth==1:
                    server_rpc_ret_str_list.append("if ("+righname+"->"+decl.name+" == NULL) {"+leftname+opname+decl.name+".twocharptr.twocharptr_len = 0;"+leftname+opname+decl.name+".twocharptr.twocharptr_val = NULL;}\n")

                else:
                    server_rpc_ret_str_list.append("if ("+righname+opname+decl.name+" == NULL) {"+leftname+opname+decl.name+".twocharptr.twocharptr_len = 0;"+leftname+opname+decl.name+".twocharptr.twocharptr_val = NULL;}\n")
                server_rpc_ret_str_list.append("else {"+leftname+opname+decl.name+".twocharptr.twocharptr_len = 1;"+leftname+opname+decl.name+".twocharptr.twocharptr_val = malloc("+leftname+opname+decl.name+".twocharptr.twocharptr_len * sizeof(char_ptr));\n")
                server_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+".twocharptr.twocharptr_val == NULL) {perror(\"malloc failed\");exit(1);}")
                server_rpc_ret_str_list.append("\tfor (int i1 = 0; i1 < "+leftname+opname+decl.name+".twocharptr.twocharptr_len; i1++) {")
                if depth==1:
                    server_rpc_ret_str_list.append("\t\tif ("+righname+"->"+decl.name+"[i1] == NULL) {"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i1].charptr.charptr_len = 0;"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i1].charptr.charptr_val = NULL;}\n")
                else:
                    server_rpc_ret_str_list.append("\t\tif ("+righname+opname+decl.name+"[i] == NULL) {"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = 0;"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;}\n")
                if depth==1:
                    server_rpc_ret_str_list.append("\t\telse {"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i1].charptr.charptr_len = strlen("+righname+"->"+decl.name+"[i1]) + 1;"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i1].charptr.charptr_val = "+righname+"->"+decl.name+"[i1];}\n")
                else:
                    server_rpc_ret_str_list.append("\t\telse {"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = strlen("+righname+opname+decl.name+"[i]) + 1;"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = "+righname+opname+decl.name+"[i];}\n")
                server_rpc_ret_str_list.append("\t}")
                server_rpc_ret_str_list.append("}")
                
            elif decltypename=="struct stat *":
                pass
            else:
                typename=decltypename.split(" ")[-2]
                if typename in basic_type or typename =="ngx_atomic_t" or typename=="ngx_uint_t":
                    if depth==1:
                        recur_dealwith_basicptr_server_retvalue(leftname+opname+decl.name,righname+"->"+opname+decl.name,".",typename,server_rpc_ret_str_list)
                    else:
                        recur_dealwith_basicptr_server_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",typename,server_rpc_ret_str_list)

                elif typename in typedef_dict:
                    if typedef_dict[typename]['isenum']:
                        if depth==1:
                            recur_dealwith_basicptr_server_retvalue(leftname+opname+decl.name,righname+"->"+opname+decl.name,".",typename,server_rpc_ret_str_list)
                        else:
                            recur_dealwith_basicptr_server_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",typename,server_rpc_ret_str_list)
                    elif typedef_dict[typename]['isstruct']:
                        if depth==1:
                            server_rpc_ret_str_list.append("if ("+righname+"->"+opname+decl.name+" == NULL) {"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len = 0;"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val = NULL;}")
                        else:
                            server_rpc_ret_str_list.append("if ("+righname+opname+decl.name+" == NULL) {"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len = 0;"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val = NULL;}")
                        server_rpc_ret_str_list.append("else {"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len = 1;"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val = malloc("+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len * sizeof("+typename+"_rpc));")
                        server_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val == NULL) {perror(\"malloc failed\");exit(1);}")
                        server_rpc_ret_str_list.append("\tfor (int i1 = 0; i1 < "+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len; i1++) {")
                        if depth==1:
                            recur_dealwith_struct_server_retvalue(typedef_dict[typename]['ast'], leftname+opname+decl.name+opname+typename+"_rpc_ptr_val[i1]", righname+"->"+opname+decl.name, ".",server_rpc_ret_str_list,depth+1,processed_types)
                        else:
                            recur_dealwith_struct_server_retvalue(typedef_dict[typename]['ast'], leftname+opname+decl.name+opname+typename+"_rpc_ptr_val[i1]", righname+opname+decl.name, ".",server_rpc_ret_str_list,depth+1,processed_types)
                        server_rpc_ret_str_list.append("\t}")
                        server_rpc_ret_str_list.append("}")
                    else:
                        if isinstance(typedef_dict[typename]['ast'].type.type,c_ast.Union):
                            continue
                        if isinstance(typedef_dict[typename]['ast'].type.type,c_ast.FuncDecl):
                            continue
                        if not isinstance(typedef_dict[typename]['ast'].type.type,c_ast.IdentifierType):
                            continue
                        typename=typedef_dict[typename]['ast'].type.type.names[0]
                        if typename in basic_type:
                            if depth==1:
                                recur_dealwith_basicptr_server_retvalue(leftname+opname+decl.name,righname+"->"+opname+decl.name,".",typename,server_rpc_ret_str_list)
                            else:
                                recur_dealwith_basicptr_server_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",typename,server_rpc_ret_str_list)
                        elif typename in struct_dict:
                            if typename=="stat":
                                pass
                            elif struct_dict[typename]['isenum']:
                                if depth==1:
                                    recur_dealwith_basicptr_server_retvalue(leftname+opname+decl.name,righname+"->"+opname+decl.name,".",typename,server_rpc_ret_str_list)
                                else:
                                    recur_dealwith_basicptr_server_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",typename,server_rpc_ret_str_list)
                            elif struct_dict[typename]['isstruct']:
                                if depth==1:
                                    server_rpc_ret_str_list.append("if ("+righname+"->"+opname+decl.name+opname+typename+" == NULL) {"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len = 0;"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val = NULL;}")
                                else:
                                    server_rpc_ret_str_list.append("if ("+righname+opname+decl.name+" == NULL) {"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len = 0;"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val = NULL;}")
                                server_rpc_ret_str_list.append("else {"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len = 1;"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val = malloc("+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len * sizeof("+typename+"_rpc));")
                                server_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val == NULL) {perror(\"malloc failed\");exit(1);}")
                                server_rpc_ret_str_list.append("\tfor (int i1 = 0; i1 < "+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len; i1++) {")
                                if depth==1:
                                    recur_dealwith_struct_server_retvalue(struct_dict[typename]['ast'], leftname+opname+decl.name+opname+typename+"_rpc_ptr_val[i1]", righname+"->"+opname+decl.name, ".", server_rpc_ret_str_list, depth+1,processed_types)
                                else:

                                    recur_dealwith_struct_server_retvalue(struct_dict[typename]['ast'], leftname+opname+decl.name+opname+typename+"_rpc_ptr_val[i1]", righname+opname+decl.name, ".", server_rpc_ret_str_list, depth+1,processed_types)
                                server_rpc_ret_str_list.append("\t}")
                                server_rpc_ret_str_list.append("}")
                            else:
                                pass
                elif typename in struct_dict:
                    if typename=="stat":
                        pass
                    elif struct_dict[typename]['isenum']:
                        if depth==1:
                            recur_dealwith_basicptr_server_retvalue(leftname+opname+decl.name,righname+"->"+opname+decl.name,".",typename,server_rpc_ret_str_list)
                        else:
                            recur_dealwith_basicptr_server_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",typename,server_rpc_ret_str_list)
                    elif struct_dict[typename]['isstruct']:
                        if depth==1:
                            server_rpc_ret_str_list.append("if ("+righname+opname+decl.name+" == NULL) {"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len = 0;"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val = NULL;}\n")
                        else:
                            server_rpc_ret_str_list.append("if ("+righname+opname+decl.name+" == NULL) {"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len = 0;"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val = NULL;}\n")
                        server_rpc_ret_str_list.append("else {"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len = 1;"+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val = malloc("+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len * sizeof("+typename+"_rpc));\n")
                        server_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+opname+typename+"_rpc_ptr_val == NULL) {perror(\"malloc failed\");exit(1);\n}")
                        server_rpc_ret_str_list.append("\tfor (int i1 = 0; i1 < "+leftname+opname+decl.name+opname+typename+"_rpc_ptr_len; i1++) {\n")
                        if depth==1:
                            recur_dealwith_struct_server_retvalue(struct_dict[typename]['ast'], leftname+opname+decl.name+opname+typename+"_rpc_ptr_val[i1]", righname+"->"+opname+decl.name, ".", server_rpc_ret_str_list, depth+1,processed_types)
                        else:
                            recur_dealwith_struct_server_retvalue(struct_dict[typename]['ast'], leftname+opname+decl.name+opname+typename+"_rpc_ptr_val[i1]", righname+opname+decl.name, ".", server_rpc_ret_str_list, depth+1,processed_types)
                        server_rpc_ret_str_list.append("\t}")
                        server_rpc_ret_str_list.append("}")
                    else:
                        pass

def recur_dealwith_struct_client_retvalue(structast,leftname,righname,opname,client_rpc_ret_str_list,processed_types=None,depth=1):
    if processed_types is None:
        processed_types = set()

    # 如果是结构体类型，获取其名称
    if isinstance(structast, c_ast.Typedef):
        struct_name = structast.name
    elif hasattr(structast, 'name') and structast.name:
        struct_name = structast.name
    else:
        struct_name = str(id(structast))  # 没有名称时使用对象ID
    
    # 检查是否已处理过该类型，避免循环
    if struct_name in processed_types:
        return
    
    processed_types.add(struct_name)

    #检查是否是typedef
    if isinstance(structast,c_ast.Typedef):
        structast=structast.type.type
    if structast.decls is None:
        if structast.name in struct_dict:
            structast=struct_dict[structast.name]['ast']
        else:
            pass
    if structast.name=="stat":
        return
    for decl in structast.decls:
        decltype=decl.type
        decltypename=generator.visit(decltype)
        if isinstance(decltype,c_ast.TypeDecl):
            decltypename=decltypename.split(" ")[-1]
            if decltypename in basic_type:
                client_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
            elif decltypename=="stat":
                recur_dealwith_stat_client_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",client_rpc_ret_str_list)
            elif decltypename in typedef_dict:
                if typedef_dict[decltypename]['isenum']:
                    client_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                elif typedef_dict[decltypename]['isstruct']:
                    recur_dealwith_struct_client_retvalue(typedef_dict[decltypename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",client_rpc_ret_str_list,processed_types)
                else:
                    if isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.Union):
                        continue
                    if isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.FuncDecl):
                        continue
                    # print(generator.visit(typedef_dict[decltypename]['ast'].type))
                    # print(generator.visit(typedef_dict[decltypename]['ast'].type.type))
                    #跳过void *
                    if (generator.visit(typedef_dict[decltypename]['ast'].type)=="void *"):
                        continue
                    
                    typename=typedef_dict[decltypename]['ast'].type.type.names[0]
                    if typename in basic_type:
                        client_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                    elif typename in struct_dict: 
                        if typename=="stat":
                            recur_dealwith_stat_client_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",client_rpc_ret_str_list)
                        elif struct_dict[typename]['isenum']:
                            client_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                        elif struct_dict[typename]['isstruct']:
                            recur_dealwith_struct_client_retvalue(struct_dict[typename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",client_rpc_ret_str_list,processed_types)
                        else: 
                            pass
            elif decltypename in struct_dict:
                if decltypename=="stat":
                    recur_dealwith_stat_client_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",client_rpc_ret_str_list)
                elif struct_dict[decltypename]['isenum']:
                    client_rpc_ret_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                elif struct_dict[decltypename]['isstruct']:
                    recur_dealwith_struct_client_retvalue(struct_dict[decltypename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",client_rpc_ret_str_list,processed_types)
                else:
                    pass
        

        elif isinstance(decltype,c_ast.PtrDecl):
            if decltypename=="char *" or decltypename=="const char *" or decltypename=="FILE *":
                client_rpc_ret_str_list.append("if ("+righname+opname+decl.name+".charptr.charptr_val == NULL) {"+leftname+opname+decl.name+" = NULL;}")
                client_rpc_ret_str_list.append("else {"+leftname+opname+decl.name+" = strdup("+righname+opname+decl.name+".charptr.charptr_val);}")
            elif decltypename=="void *":
                client_rpc_ret_str_list.append("if ("+righname+opname+decl.name+"."+decl.name+"_val == NULL) {"+leftname+opname+decl.name+" = NULL;}")
                client_rpc_ret_str_list.append("else {"+leftname+opname+decl.name+" = (char *)malloc("+righname+opname+decl.name+"."+decl.name+"_len * sizeof(char));")
                client_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+" == NULL) {")
                client_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
                client_rpc_ret_str_list.append("\t\texit(1);")
                client_rpc_ret_str_list.append("\t}")
                client_rpc_ret_str_list.append("\tmemcpy("+leftname+opname+decl.name+", "+righname+opname+decl.name+"."+decl.name+"_val, "+righname+opname+decl.name+"."+decl.name+"_len);")
                client_rpc_ret_str_list.append("}")  # 这里的else是为了处理非NULL的情况，malloc和memcpy都在else中
                
            elif decltypename=="char **" or decltypename=="const char **":
                client_rpc_ret_str_list.append("if ("+righname+opname+decl.name+".twocharptr.twocharptr_val == NULL) {"+leftname+opname+decl.name+" = NULL;}")
                client_rpc_ret_str_list.append("else {")
                client_rpc_ret_str_list.append("\t"+leftname+opname+decl.name+" = (char **)malloc("+righname+opname+decl.name+".twocharptr.twocharptr_len * sizeof(char *));")
                client_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+" == NULL) {")
                client_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
                client_rpc_ret_str_list.append("\t\texit(1);")
                client_rpc_ret_str_list.append("\t}")
                client_rpc_ret_str_list.append("\tfor (int i1 = 0; i1 < "+righname+opname+decl.name+".twocharptr.twocharptr_len; i1++) {")
                client_rpc_ret_str_list.append("\t\tif ("+righname+opname+decl.name+".twocharptr.twocharptr_val[i1].charptr.charptr_val == NULL) {"+leftname+opname+decl.name+"[i1] = NULL;}")
                client_rpc_ret_str_list.append("\t\telse {")
                client_rpc_ret_str_list.append("\t\t\t"+leftname+opname+decl.name+"[i1] = strdup("+righname+opname+decl.name+".twocharptr.twocharptr_val[i1].charptr.charptr_val);")
                client_rpc_ret_str_list.append("\t\t}")
                client_rpc_ret_str_list.append("\t}")
                client_rpc_ret_str_list.append("}")
                
            elif decltypename=="struct stat *":
                pass
            else:
                typename=decltypename.split(" ")[-2]
                if typename in basic_type:
                    recur_dealwith_basicptr_client_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",typename,client_rpc_ret_str_list)

                elif typename in typedef_dict:
                    if typedef_dict[typename]['isenum']:
                        recur_dealwith_basicptr_client_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",typename,client_rpc_ret_str_list)
                    elif typedef_dict[typename]['isstruct']:
                        
                        client_rpc_ret_str_list.append("if ("+righname+opname+decl.name+opname+typename+"_rpc_ptr_val == NULL) {"+leftname+opname+decl.name+" = NULL;}")
                        client_rpc_ret_str_list.append("else {")
                        client_rpc_ret_str_list.append("\t"+leftname+opname+decl.name+" = ("+typename+" *)malloc("+righname+opname+decl.name+opname+typename+"_rpc_ptr_len * sizeof("+typename+"));")
                        client_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+" == NULL) {")
                        client_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
                        client_rpc_ret_str_list.append("\t\texit(1);")
                        client_rpc_ret_str_list.append("\t}")
                        client_rpc_ret_str_list.append("\tfor (int i1 = 0; i1 < "+righname+opname+decl.name+opname+typename+"_rpc_ptr_len; i1++) {")
                        recur_dealwith_struct_client_retvalue(typedef_dict[typename]['ast'], leftname+opname+decl.name+"[i1]", righname+opname+decl.name+opname+typename+"_rpc_ptr_val[i1]", ".",client_rpc_ret_str_list,processed_types)
                        client_rpc_ret_str_list.append("\t}")
                        client_rpc_ret_str_list.append("}")
                    else:
                        #跳过Union和FuncDecl,void *
                        if isinstance(typedef_dict[typename]['ast'].type.type,c_ast.Union):
                            continue
                        if isinstance(typedef_dict[typename]['ast'].type.type,c_ast.FuncDecl):
                            continue
                        if (generator.visit(typedef_dict[typename]['ast'].type)=="void *"):
                            continue
                        typename=typedef_dict[typename]['ast'].type.type.names[0]
                        if typename in basic_type:
                            recur_dealwith_basicptr_client_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",typename,client_rpc_ret_str_list)
                        elif typename in struct_dict:
                            if typename=="stat":
                                pass
                            elif struct_dict[typename]['isenum']:
                                recur_dealwith_basicptr_client_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",typename,client_rpc_ret_str_list)
                            elif struct_dict[typename]['isstruct']:
                                client_rpc_ret_str_list.append("if ("+righname+opname+decl.name+opname+typename+"_rpc_ptr_val == NULL) {"+leftname+opname+decl.name+" = NULL;}")
                                client_rpc_ret_str_list.append("else {")
                                client_rpc_ret_str_list.append("\t"+leftname+opname+decl.name+" = (struct "+typename+" *)malloc("+righname+opname+decl.name+opname+typename+"_rpc_ptr_len * sizeof(struct "+typename+"));")
                                client_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+" == NULL) {")
                                client_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
                                client_rpc_ret_str_list.append("\t\texit(1);")
                                client_rpc_ret_str_list.append("\t}")
                                client_rpc_ret_str_list.append("\tfor (int i1 = 0; i1 < "+righname+opname+decl.name+opname+typename+"_rpc_ptr_len; i1++) {")
                                recur_dealwith_struct_client_retvalue(struct_dict[typename]['ast'], leftname+opname+decl.name+"[i1]", righname+opname+decl.name+opname+typename+"_rpc_ptr_val[i1]", "->", client_rpc_ret_str_list,processed_types,depth+1)
                                client_rpc_ret_str_list.append("\t}")
                                client_rpc_ret_str_list.append("}")
                            else:
                                pass
                elif typename in struct_dict:
                    if typename=="stat":
                        pass
                    elif struct_dict[typename]['isenum']:
                        recur_dealwith_basicptr_client_retvalue(leftname+opname+decl.name,righname+opname+decl.name,".",typename,client_rpc_ret_str_list)
                    elif struct_dict[typename]['isstruct']:
                        client_rpc_ret_str_list.append("if ("+righname+opname+decl.name+opname+typename+"_rpc_ptr_val == NULL) {"+leftname+opname+decl.name+" = NULL;}")
                        client_rpc_ret_str_list.append("else {")
                        client_rpc_ret_str_list.append("\t"+leftname+opname+decl.name+" = (struct "+typename+" *)malloc("+righname+opname+decl.name+opname+typename+"_rpc_ptr_len * sizeof( struct "+typename+"));")
                        client_rpc_ret_str_list.append("\tif ("+leftname+opname+decl.name+" == NULL) {")
                        client_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
                        client_rpc_ret_str_list.append("\t\texit(1);")
                        client_rpc_ret_str_list.append("\t}")
                        client_rpc_ret_str_list.append("\tfor (int i1 = 0; i1 < "+righname+opname+decl.name+opname+typename+"_rpc_ptr_len; i1++) {")
                        recur_dealwith_struct_client_retvalue(struct_dict[typename]['ast'], leftname+opname+decl.name+"[i1]", righname+opname+decl.name+opname+typename+"_rpc_ptr_val[i1]", ".", client_rpc_ret_str_list,processed_types,depth+1)
                        client_rpc_ret_str_list.append("\t}")
                        client_rpc_ret_str_list.append("}")
                    else:
                        pass

def dealwith_structptr_client_server_retvalue(typename,server_rpc_ret_str_list,client_rpc_ret_str_list):
    #服务端封装返回值
    server_rpc_ret_str_list.append("if (sensefun_ret == NULL) {result->ret0."+typename+"rpc_ptr_len = 0;relust->ret0."+typename+"rpc_ptr_val = NULL;}")
    server_rpc_ret_str_list.append("else {")
    server_rpc_ret_str_list.append("\tresult->ret0."+typename+"rpc_ptr_len = 1;")
    server_rpc_ret_str_list.append("\tfor (int i = 0; i < result->ret0."+typename+"rpc_ptr_len; i++) {")
    recur_dealwith_struct_server_retvalue(struct_dict[typename]['ast'], "result->ret0."+typename+"rpc_ptr_val[i]", "sensefun_ret[i]", "->", server_rpc_ret_str_list,0,None)
    server_rpc_ret_str_list.append("\t}")
    #客户端解封装返回值
    client_rpc_ret_str_list.append("if (result_rpc->ret0."+typename+"rpc_ptr_val == NULL) {sensefun_ret = NULL;}")
    client_rpc_ret_str_list.append("else {")
    client_rpc_ret_str_list.append("\tsensefun_ret = ("+typename+" *)malloc(result_rpc->ret0."+typename+"rpc_ptr_len * sizeof("+typename+"));")
    client_rpc_ret_str_list.append("\tif (sensefun_ret == NULL) {")
    client_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
    client_rpc_ret_str_list.append("\t\texit(1);")
    client_rpc_ret_str_list.append("\t}")
    client_rpc_ret_str_list.append("\tfor (int i = 0; i < result_rpc->ret0."+typename+"rpc_ptr_len; i++) {")
    recur_dealwith_struct_client_retvalue(struct_dict[typename]['ast'], "sensefun_ret[i]", "result_rpc->ret0."+typename+"rpc_ptr_val[i]", "->", client_rpc_ret_str_list,None,1)

def recur_dealwith_stat_server_param(leftname,righname,opname,unmashal_str_list):
    unmashal_str_list.append(leftname+opname+"st_dev="+righname+opname+"st_dev;")
    unmashal_str_list.append(leftname+opname+"st_ino="+righname+opname+"st_ino;")
    unmashal_str_list.append(leftname+opname+"st_mode="+righname+opname+"st_mode;")
    unmashal_str_list.append(leftname+opname+"st_nlink="+righname+opname+"st_nlink;")
    unmashal_str_list.append(leftname+opname+"st_uid="+righname+opname+"st_uid;")
    unmashal_str_list.append(leftname+opname+"st_gid="+righname+opname+"st_gid;")
    unmashal_str_list.append(leftname+opname+"st_rdev="+righname+opname+"st_rdev;")
    unmashal_str_list.append(leftname+opname+"st_size="+righname+opname+"st_size;")
    unmashal_str_list.append(leftname+opname+"st_blksize="+righname+opname+"st_blksize;")
    unmashal_str_list.append(leftname+opname+"st_blocks="+righname+opname+"st_blocks;")
    unmashal_str_list.append(leftname+opname+"st_atime="+righname+opname+"st_atime;")
    unmashal_str_list.append(leftname+opname+"st_mtime="+righname+opname+"st_mtime;")
    unmashal_str_list.append(leftname+opname+"st_ctime="+righname+opname+"st_ctime;")


def recur_dealwith_basicptr_server_param(leftname,righname,opname,typename, unmashal_str_list):
    unmashal_str_list.append("if ("+righname+"_rpc."+typename+"_ptr_val == NULL) {"+leftname+"=NULL;}")
    unmashal_str_list.append("else {"+leftname+"=("+typename+" *)malloc("+righname+"_rpc."+typename+"_ptr_len * sizeof("+typename+"));")
    unmashal_str_list.append("\tif ("+leftname+" == NULL) {")
    unmashal_str_list.append("\t\tperror(\"malloc failed\");")
    unmashal_str_list.append("\t\texit(1);")
    unmashal_str_list.append("\t}")
    unmashal_str_list.append("\tfor (int i = 0; i < "+righname+"_rpc."+typename+"_ptr_len; i++) {")
    unmashal_str_list.append("\t\t"+leftname+"[i] = "+righname+"_rpc."+typename+"_ptr_val[i];")
    unmashal_str_list.append("\t}")
    unmashal_str_list.append("}")


def dealwith_basic_client_server_param(ptype,param,paramcount,wrapper_str_list,clnt_rpc_call_param_list,svc_param_list,unmashal_str_list):
        # 客户端封装数据
    # print("ptype:",ptype)
    # print("param:",param)
    if ptype =="void":
        return
    wrapper_str_list.append(ptype+" "+param.name+"_rpc;")
    clnt_rpc_call_param_list.append(param.name+"_rpc")
    wrapper_str_list.append(param.name+"_rpc="+param.name+";")
    #服务端解封装数据
    svc_param_list.append(ptype+" arg"+str(paramcount))
    #int temp=arg1;
    unmashal_str_list.append(ptype+" "+param.name+"=arg"+str(paramcount)+";")

def dealwith_stat_client_server_param(param,paramcount,wrapper_str_list,clnt_rpc_call_param_list,svc_param_list,unmashal_str_list):
    """struct stat_rpc http_loop_sense_1_1_st;
    memset(&http_loop_sense_1_1_st, 0, sizeof(stat_rpc));
    //强制类型转换
    // memcpy(&http_loop_sense_1_1_st, &st, sizeof(stat_rpc));
    http_loop_sense_1_1_st.st_dev=st.st_dev;
    http_loop_sense_1_1_st.st_ino=st.st_ino;
    http_loop_sense_1_1_st.st_mode=st.st_mode;
    http_loop_sense_1_1_st.st_nlink=st.st_nlink;
    http_loop_sense_1_1_st.st_uid=st.st_uid;
    http_loop_sense_1_1_st.st_gid=st.st_gid;
    http_loop_sense_1_1_st.st_rdev=st.st_rdev;
    http_loop_sense_1_1_st.st_size=st.st_size;
    http_loop_sense_1_1_st.st_blksize=st.st_blksize;
    http_loop_sense_1_1_st.st_blocks=st.st_blocks;
    http_loop_sense_1_1_st.st_atim=st.st_atime;
    http_loop_sense_1_1_st.st_mtim=st.st_mtime;
    http_loop_sense_1_1_st.st_ctim=st.st_ctime;
    """
    # 客户端封装数据
    wrapper_str_list.append(" stat_rpc "+param.name+"_rpc;")
    clnt_rpc_call_param_list.append(param.name+"_rpc")
    wrapper_str_list.append("memset(&"+param.name+"_rpc, 0, sizeof( stat_rpc));")
    wrapper_str_list.append(param.name+"_rpc.st_dev="+param.name+".st_dev;")
    wrapper_str_list.append(param.name+"_rpc.st_ino="+param.name+".st_ino;")
    wrapper_str_list.append(param.name+"_rpc.st_mode="+param.name+".st_mode;")
    wrapper_str_list.append(param.name+"_rpc.st_nlink="+param.name+".st_nlink;")
    wrapper_str_list.append(param.name+"_rpc.st_uid="+param.name+".st_uid;")
    wrapper_str_list.append(param.name+"_rpc.st_gid="+param.name+".st_gid;")
    wrapper_str_list.append(param.name+"_rpc.st_rdev="+param.name+".st_rdev;")
    wrapper_str_list.append(param.name+"_rpc.st_size="+param.name+".st_size;")
    wrapper_str_list.append(param.name+"_rpc.st_blksize="+param.name+".st_blksize;")
    wrapper_str_list.append(param.name+"_rpc.st_blocks="+param.name+".st_blocks;")
    wrapper_str_list.append(param.name+"_rpc.st_atim="+param.name+".st_atime;")
    wrapper_str_list.append(param.name+"_rpc.st_mtim="+param.name+".st_mtime;")
    wrapper_str_list.append(param.name+"_rpc.st_ctim="+param.name+".st_ctime;")
    
    #服务端解封装数据
    svc_param_list.append("stat_rpc arg"+str(paramcount))
    unmashal_str_list.append("struct stat "+param.name+";")
    unmashal_str_list.append(param.name+".st_dev=arg"+str(paramcount)+".st_dev;")
    unmashal_str_list.append(param.name+".st_ino=arg"+str(paramcount)+".st_ino;")
    unmashal_str_list.append(param.name+".st_mode=arg"+str(paramcount)+".st_mode;")
    unmashal_str_list.append(param.name+".st_nlink=arg"+str(paramcount)+".st_nlink;")
    unmashal_str_list.append(param.name+".st_uid=arg"+str(paramcount)+".st_uid;")
    unmashal_str_list.append(param.name+".st_gid=arg"+str(paramcount)+".st_gid;")
    unmashal_str_list.append(param.name+".st_rdev=arg"+str(paramcount)+".st_rdev;")
    unmashal_str_list.append(param.name+".st_size=arg"+str(paramcount)+".st_size;")
    unmashal_str_list.append(param.name+".st_blksize=arg"+str(paramcount)+".st_blksize;")
    unmashal_str_list.append(param.name+".st_blocks=arg"+str(paramcount)+".st_blocks;")
    unmashal_str_list.append(param.name+".st_atime=arg"+str(paramcount)+".st_atim;")
    unmashal_str_list.append(param.name+".st_mtime=arg"+str(paramcount)+".st_mtim;")
    unmashal_str_list.append(param.name+".st_ctime=arg"+str(paramcount)+".st_ctim;")


def recur_dealwith_struct_ptr_client_param(structast,leftname,righname,opname,typename,declname,wrapper_str_list,depth=1,processed_types=None):
    # 如果没有传递processed_types，则创建新的集合
    if processed_types is None:
        processed_types = set()
        
    wrapper_str_list.append("if ("+righname+opname+declname+" == NULL) {"+leftname+opname+declname+opname+typename+"_rpc_ptr_len = 0;"+leftname+opname+declname+opname+typename+"_rpc_ptr_val = NULL;}")
    wrapper_str_list.append("else {"+leftname+opname+declname+opname+typename+"_rpc_ptr_len = 1;"+leftname+opname+declname+opname+typename+"_rpc_ptr_val = malloc("+leftname+opname+declname+opname+typename+"_rpc_ptr_len * sizeof("+typename+"_rpc));")
    wrapper_str_list.append("\tif ("+leftname+opname+declname+opname+typename+"_rpc_ptr_val == NULL) {")
    wrapper_str_list.append("\t\tperror(\"malloc failed\");")
    wrapper_str_list.append("\t\texit(1);")
    wrapper_str_list.append("\t}")
    # wrapper_str_list.append("\tfor (int i = 0; i < "+leftname+opname+declname+"_rpc_ptr_len; i++) {")
    wrapper_str_list.append(f"\tfor (int i{depth} = 0; i{depth} < {leftname}{opname}{declname}{opname}{typename}_rpc_ptr_len; i{depth}++) {{")
    recur_dealwith_struct_client_param(structast, leftname+opname+declname+opname+typename+"_rpc_ptr_val[i"+str(depth)+"]", righname+opname+declname+"[i"+str(depth)+"]", ".",wrapper_str_list, depth, processed_types)
    wrapper_str_list.append("\t}")
    wrapper_str_list.append("}")

def recur_dealwith_stat_client_param(leftname,righname,opname,wrapper_str_list):
    wrapper_str_list.append(leftname+opname+"st_dev="+righname+opname+"st_dev;")
    wrapper_str_list.append(leftname+opname+"st_ino="+righname+opname+"st_ino;")
    wrapper_str_list.append(leftname+opname+"st_mode="+righname+opname+"st_mode;")
    wrapper_str_list.append(leftname+opname+"st_nlink="+righname+opname+"st_nlink;")
    wrapper_str_list.append(leftname+opname+"st_uid="+righname+opname+"st_uid;")
    wrapper_str_list.append(leftname+opname+"st_gid="+righname+opname+"st_gid;")
    wrapper_str_list.append(leftname+opname+"st_rdev="+righname+opname+"st_rdev;")
    wrapper_str_list.append(leftname+opname+"st_size="+righname+opname+"st_size;")
    wrapper_str_list.append(leftname+opname+"st_blksize="+righname+opname+"st_blksize;")
    wrapper_str_list.append(leftname+opname+"st_blocks="+righname+opname+"st_blocks;")
    wrapper_str_list.append(leftname+opname+"st_atime="+righname+opname+"st_atime;")
    wrapper_str_list.append(leftname+opname+"st_mtime="+righname+opname+"st_mtime;")
    wrapper_str_list.append(leftname+opname+"st_ctime="+righname+opname+"st_ctime;")

def recur_dealwith_basicptr_client_param(leftname,righname,opname,typename,wrapper_str_list):
    wrapper_str_list.append("if ("+righname+"== NULL) {"+leftname+"_rpc."+typename+"_ptr_len = 0;"+leftname+"_rpc."+typename+"_ptr_val = NULL;}")
    wrapper_str_list.append("else {"+leftname+"_rpc."+typename+"_ptr_len = 1;"+leftname+"_rpc."+typename+"_ptr_val = malloc("+leftname+"_rpc."+typename+"_ptr_len * sizeof("+typename+"));")
    wrapper_str_list.append("\tif ("+leftname+"_rpc."+typename+"_ptr_val == NULL) {")
    wrapper_str_list.append("\t\tperror(\"malloc failed\");")
    wrapper_str_list.append("\t\texit(1);")
    wrapper_str_list.append("\t}")
    wrapper_str_list.append("\tfor (int i = 0; i < "+leftname+"_rpc."+typename+"_ptr_len; i++) {")
    wrapper_str_list.append("\t\t"+leftname+"_rpc."+typename+"_ptr_val[i] = "+righname+"[i];")
    wrapper_str_list.append("\t}")
    wrapper_str_list.append("}")


def recur_dealwith_struct_client_param(structast,leftname,righname,opname,wrapper_str_list,depth=0,processed_types=None):
    #这个地方structast可能是一个结构体的ast，也可能是一个typedef的ast
    # print(structast)
    
    # 初始化已处理类型集合
    if processed_types is None:
        processed_types = set()
    
    # 获取结构体名称用于循环检测
    if isinstance(structast, c_ast.Typedef):
        struct_name = structast.name
        structast = structast.type.type
    elif hasattr(structast, 'name') and structast.name:
        struct_name = structast.name
    else:
        struct_name = str(id(structast))  # 没有名称时使用对象ID
    
    # 检查是否已处理过该类型，避免循环
    if struct_name in processed_types:
        return
    
    processed_types.add(struct_name)
    
    if structast.decls is None:
        if structast.name in struct_dict:
            structast=struct_dict[structast.name]['ast']
        else:
            return
        
    for decl in structast.decls:
        decltype=decl.type
        decltypename=generator.visit(decltype)
        if isinstance(decltype,c_ast.TypeDecl):
            decltypename=decltypename.split(" ")[-1]
            # if decl.name=="scheme" and "--debug" in sys.argv:
            #     pdb.set_trace()
                # print(decl)
            if decltypename in basic_type:
                wrapper_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
            elif decltypename=="stat":
                recur_dealwith_stat_client_param(leftname+opname+decl.name,righname+opname+decl.name,".",wrapper_str_list)
            elif decltypename in typedef_dict:
                if typedef_dict[decltypename]['isenum']:
                    wrapper_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                elif typedef_dict[decltypename]['isstruct']:
                    recur_dealwith_struct_client_param(typedef_dict[decltypename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",wrapper_str_list, depth, processed_types)
                else:
                    if isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.Union):
                        continue
                    if isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.FuncDecl):
                        continue
                    if not isinstance(typedef_dict[decltypename]['ast'].type.type,c_ast.IdentifierType):
                        continue
                    typename=typedef_dict[decltypename]['ast'].type.type.names[0]
                    if typename in basic_type:
                        wrapper_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                    elif typename in struct_dict:
                        if typename=="stat":
                            recur_dealwith_stat_client_param(leftname+opname+decl.name,righname+opname+decl.name,".",wrapper_str_list)
                        elif struct_dict[typename]['isenum']:
                            wrapper_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                        elif struct_dict[typename]['isstruct']:
                            recur_dealwith_struct_client_param(struct_dict[typename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",wrapper_str_list, depth, processed_types)
                        else:
                            pass
            elif decltypename in struct_dict:
                if decltypename=="stat":
                    recur_dealwith_stat_client_param(leftname+opname+decl.name,righname+opname+decl.name,".",wrapper_str_list)
                elif struct_dict[decltypename]['isenum']:
                    wrapper_str_list.append(leftname+opname+decl.name+"="+righname+opname+decl.name+";")
                elif struct_dict[decltypename]['isstruct']:
                    recur_dealwith_struct_client_param(struct_dict[decltypename]['ast'],leftname+opname+decl.name,righname+opname+decl.name,".",wrapper_str_list, depth, processed_types)
                else:
                    pass
        

        elif isinstance(decltype,c_ast.PtrDecl):
            if decltypename=="char *" or decltypename=="const char *" or decltypename=="FILE *":
                wrapper_str_list.append("if ("+righname+opname+decl.name+" == NULL) {"+leftname+opname+decl.name+".charptr.charptr_len = 0;"+leftname+opname+decl.name+".charptr.charptr_val = NULL;}")
                wrapper_str_list.append("else {"+leftname+opname+decl.name+".charptr.charptr_len = strlen("+righname+opname+decl.name+") + 1;\n")
                wrapper_str_list.append("\t"+leftname+opname+decl.name+".charptr.charptr_val = malloc("+leftname+opname+decl.name+".charptr.charptr_len * sizeof(char));")
                wrapper_str_list.append("\tif ("+leftname+opname+decl.name+".charptr.charptr_val == NULL) {")
                wrapper_str_list.append("\t\tperror(\"malloc failed\");")
                wrapper_str_list.append("\t\texit(1);")
                wrapper_str_list.append("\t}")
                wrapper_str_list.append("\tmemcpy("+leftname+opname+decl.name+".charptr.charptr_val, "+righname+opname+decl.name+", "+leftname+opname+decl.name+".charptr.charptr_len);")
                wrapper_str_list.append("}")
        
            
            elif decltypename=="void *":
                pass
            elif decltypename=="char **" or decltypename=="const char **":
                wrapper_str_list.append("if ("+righname+opname+decl.name+" == NULL) {"+leftname+opname+decl.name+".twocharptr.twocharptr_len = 0;"+leftname+opname+decl.name+".twocharptr.twocharptr_val = NULL;}")
                wrapper_str_list.append("else {"+leftname+opname+decl.name+".twocharptr.twocharptr_len = 1;"+leftname+opname+decl.name+".twocharptr.twocharptr_val = malloc("+leftname+opname+decl.name+".twocharptr.twocharptr_len * sizeof(char_ptr));")
                wrapper_str_list.append("\tif ("+leftname+opname+decl.name+".twocharptr.twocharptr_val == NULL) {")
                wrapper_str_list.append("\t\tperror(\"malloc failed\");")
                wrapper_str_list.append("\t\texit(1);")
                wrapper_str_list.append("\t}")
                wrapper_str_list.append("\tfor (int i = 0; i < "+leftname+opname+decl.name+".twocharptr.twocharptr_len; i++) {")
                wrapper_str_list.append("\t\tif ("+righname+opname+decl.name+"[i] == NULL) {")
                wrapper_str_list.append("\t\t\t"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = 0;")
                wrapper_str_list.append("\t\t\t"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;")
                wrapper_str_list.append("\t\t} else {")
                wrapper_str_list.append("\t\t\t"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_len = strlen("+righname+opname+decl.name+"[i]) + 1;")
                wrapper_str_list.append("\t\t\t"+leftname+opname+decl.name+".twocharptr.twocharptr_val[i].charptr.charptr_val = "+righname+opname+decl.name+"[i];")
                wrapper_str_list.append("\t\t}")
                wrapper_str_list.append("\t}")
                wrapper_str_list.append("}")
                
            elif decltypename=="struct stat *":
                pass
            else:
                #二级指针跳过,FuncDecl跳过,Union跳过
                if isinstance(decltype.type,c_ast.Union):
                    continue
                if isinstance(decltype.type,c_ast.FuncDecl):
                    continue
                if (generator.visit(decltype.type)=="void *"):
                    continue
                if isinstance(decltype.type,c_ast.PtrDecl):
                    continue

                if isinstance(decltype.type.type,c_ast.Struct):
                    typename=decltype.type.type.name
                else:
                    typename=decltype.type.type.names[0]

                if typename in basic_type:
                    recur_dealwith_basicptr_client_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,wrapper_str_list)

                elif typename in typedef_dict:
                    if typedef_dict[typename]['isenum']:
                        recur_dealwith_basicptr_client_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,wrapper_str_list)
                    elif typedef_dict[typename]['isstruct']:
                        recur_dealwith_struct_ptr_client_param(typedef_dict[typename]['ast'],leftname,righname,opname,typename,decl.name,wrapper_str_list,depth+1,processed_types)
                    else:
                        if not isinstance(typedef_dict[typename]['ast'].type.type,c_ast.IdentifierType):
                            continue
                        typename=typedef_dict[typename]['ast'].type.type.names[0]
                        if typename in basic_type:
                            recur_dealwith_basicptr_client_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,wrapper_str_list)
                        elif typename in struct_dict:
                            if typename=="stat":
                                pass
                            elif struct_dict[typename]['isenum']:
                                recur_dealwith_basicptr_client_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,wrapper_str_list)
                            elif struct_dict[typename]['isstruct']:
                                recur_dealwith_struct_ptr_client_param(struct_dict[typename]['ast'],leftname,righname,opname,typename,decl.name,wrapper_str_list,depth+1,processed_types)
                            else:
                                pass
                elif typename in struct_dict:
                    if typename=="stat":
                        pass
                    elif struct_dict[typename]['isenum']:
                        recur_dealwith_basicptr_client_param(leftname+opname+decl.name,righname+opname+decl.name,".",typename,wrapper_str_list)
                    elif struct_dict[typename]['isstruct']:
                        recur_dealwith_struct_ptr_client_param(struct_dict[typename]['ast'],leftname,righname,opname,typename,decl.name,wrapper_str_list,depth+1,processed_types)
                    else:
                        pass

def dealwith_twocharptr_client_server_param_retval(param,paramcount,wrapper_str_list,clnt_rpc_call_param_list,svc_param_list,unmashal_str_list,server_rpc_ret_str_list,client_rpc_ret_str_list):
    """
    TwoCharPtr newloc_rpc;
    if (newloc == NULL) {
        newloc_rpc.twocharptr.twocharptr_len = 0;
        newloc_rpc.twocharptr.twocharptr_val = NULL;
    } else {
    newloc_rpc.twocharptr.twocharptr_len = 2;
    newloc_rpc.twocharptr.twocharptr_val = malloc(2 * sizeof(char_ptr));
    if (newloc_rpc.twocharptr.twocharptr_val == NULL) {
        perror("malloc failed");
        exit(1);
    }

    for (int i = 0; i < newloc_rpc.twocharptr.twocharptr_len; i++) {
        if (newloc[i] == NULL) {
            newloc_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
            newloc_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
        } else {
            newloc_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(newloc[i]) + 1;
            newloc_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val = newloc[i];
        }
    }
    }
    """
    #客户端封装数据
    wrapper_str_list.append("TwoCharPtr "+param.name+"_rpc;")
    clnt_rpc_call_param_list.append(param.name+"_rpc")
    wrapper_str_list.append("if("+param.name+"==NULL) {")
    wrapper_str_list.append(param.name+"_rpc.twocharptr.twocharptr_len=0;")
    wrapper_str_list.append(param.name+"_rpc.twocharptr.twocharptr_val=NULL;")
    wrapper_str_list.append("} else {")
    wrapper_str_list.append(param.name+"_rpc.twocharptr.twocharptr_len=1;")
    wrapper_str_list.append(param.name+"_rpc.twocharptr.twocharptr_val=malloc(1*sizeof(char_ptr));")
    wrapper_str_list.append("if("+param.name+"_rpc.twocharptr.twocharptr_val==NULL) {")
    wrapper_str_list.append("perror(\"malloc failed\");")
    wrapper_str_list.append("exit(1);")
    wrapper_str_list.append("}")
    wrapper_str_list.append("for(int i=0;i<"+param.name+"_rpc.twocharptr.twocharptr_len;i++) {")
    wrapper_str_list.append("if("+param.name+"[i]==NULL) {")
    wrapper_str_list.append(param.name+"_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len=0;")
    wrapper_str_list.append(param.name+"_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val=NULL;")
    wrapper_str_list.append("} else {")
    wrapper_str_list.append(param.name+"_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len=strlen("+param.name+"[i])+1;")
    wrapper_str_list.append(param.name+"_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val=malloc("+param.name+"_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));")
    wrapper_str_list.append("if("+param.name+"_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {")
    wrapper_str_list.append("perror(\"malloc failed\");")
    wrapper_str_list.append("exit(1);")
    wrapper_str_list.append("}")
    wrapper_str_list.append("memcpy("+param.name+"_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val,"+param.name+"[i],"+param.name+"_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len);")
    wrapper_str_list.append("\t\t\t}")
    wrapper_str_list.append("\t\t}")
    wrapper_str_list.append("\t}")

    
    """
    char **newloc = NULL;
    if (arg2.twocharptr.twocharptr_val != NULL) {
        newloc = (char **)malloc(arg2.twocharptr.twocharptr_len * sizeof(char *));
        if (newloc == NULL) {
            perror("malloc failed");
            exit(1);
        }
        for (int i = 0; i < 2; i++) {
            if (arg2.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                newloc[i] = NULL;
            } else {
                newloc[i] = strdup(arg2.twocharptr.twocharptr_val[i].charptr.charptr_val);
            }
        }
    }
    """
    #服务端解包数据
    svc_param_list.append("TwoCharPtr arg"+str(paramcount))
    unmashal_str_list.append("char **"+param.name+"=NULL;")
    unmashal_str_list.append("if(arg"+str(paramcount)+".twocharptr.twocharptr_val!=NULL) {")
    unmashal_str_list.append(param.name+"=(char **)malloc(arg"+str(paramcount)+".twocharptr.twocharptr_len*sizeof(char *));")
    unmashal_str_list.append("if("+param.name+"==NULL) {")
    unmashal_str_list.append("perror(\"malloc failed\");")
    unmashal_str_list.append("exit(1);")
    unmashal_str_list.append("}")
    unmashal_str_list.append("for(int i=0;i<arg"+str(paramcount)+".twocharptr.twocharptr_len;i++) {")
    unmashal_str_list.append("if(arg"+str(paramcount)+".twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {")
    unmashal_str_list.append(param.name+"[i]==NULL;")
    unmashal_str_list.append("}else{")
    unmashal_str_list.append(param.name+"[i]=malloc(arg"+str(paramcount)+".twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));")
    unmashal_str_list.append("if("+param.name+"[i]==NULL) {")
    unmashal_str_list.append("perror(\"malloc failed\");")
    unmashal_str_list.append("exit(1);")
    unmashal_str_list.append("}")
    unmashal_str_list.append("memcpy("+param.name+"[i],arg"+str(paramcount)+".twocharptr.twocharptr_val[i].charptr.charptr_val,arg"+str(paramcount)+".twocharptr.twocharptr_val[i].charptr.charptr_len);")
    unmashal_str_list.append("\t\t\t}")
    unmashal_str_list.append("\t\t}")
    unmashal_str_list.append("\t}")
    
    """
        if (newloc == NULL) {
        result->paramname.twocharptr.twocharptr_len = 0;
        result->paramname.twocharptr.twocharptr_val = NULL;
    } else {
        result->paramname.twocharptr.twocharptr_len = 2;
        result->paramname..twocharptr.twocharptr_val = malloc(2 * sizeof(char_ptr));
        if (newloc_rpc.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            exit(1);
        }

        for (int i = 0; i < result->paramname..twocharptr.twocharptr_len; i++) {
            if (newloc[i] == NULL) {
                result->paramname..twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                result->paramname..twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                result->paramname..twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(newloc[i]) + 1;
                result->paramname..twocharptr.twocharptr_val[i].charptr.charptr_val = newloc[i];
            }
        }
    }
    """

    ptypestr=generator.visit(param.type)
    if "const" not in ptypestr:
        #服务端封装返回值
        server_rpc_ret_str_list.append(space4+"if("+param.name+"==NULL) {")
        server_rpc_ret_str_list.append(space8+"result."+param.name+".twocharptr.twocharptr_len=0;")
        server_rpc_ret_str_list.append(space8+"result."+param.name+".twocharptr.twocharptr_val=NULL;")
        server_rpc_ret_str_list.append(space4+"} else {")
        server_rpc_ret_str_list.append(space8+"result."+param.name+".twocharptr.twocharptr_len=1;")
        server_rpc_ret_str_list.append(space8+"result."+param.name+".twocharptr.twocharptr_val=malloc(1*sizeof(char_ptr));")
        server_rpc_ret_str_list.append(space8+"if(result."+param.name+".twocharptr.twocharptr_val==NULL) {")
        server_rpc_ret_str_list.append(space12+"perror(\"malloc failed\");")
        server_rpc_ret_str_list.append(space12+"exit(1);")
        server_rpc_ret_str_list.append(space8+"}")
        server_rpc_ret_str_list.append(space8+"for(int i=0;i<result."+param.name+".twocharptr.twocharptr_len;i++) {")
        server_rpc_ret_str_list.append(space12+"if("+param.name+"[i]==NULL) {")
        server_rpc_ret_str_list.append(space16+"result."+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_len=0;")
        server_rpc_ret_str_list.append(space16+"result."+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_val=NULL;")
        server_rpc_ret_str_list.append(space12+"} else {")
        server_rpc_ret_str_list.append(space16+"result."+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_len=strlen("+param.name+"[i])+1;")
        server_rpc_ret_str_list.append(space16+"result."+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_val=malloc(result."+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));")
        server_rpc_ret_str_list.append(space16+"if(result."+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {")
        server_rpc_ret_str_list.append(space12+"perror(\"malloc failed\");")
        server_rpc_ret_str_list.append(space12+"exit(1);")
        server_rpc_ret_str_list.append(space16+"}")
        server_rpc_ret_str_list.append(space16+"memcpy(result."+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_val,"+param.name+"[i],result."+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_len);")
        server_rpc_ret_str_list.append(space12+"}")
        server_rpc_ret_str_list.append(space8+"}")
        server_rpc_ret_str_list.append(space4+"}")

        #客户端解封装返回值
        client_rpc_ret_str_list.append(space4+"if(result_rpc->"+param.name+".twocharptr.twocharptr_val==NULL) {")
        client_rpc_ret_str_list.append(space8+param.name+"=NULL;")
        client_rpc_ret_str_list.append(space4+"} else {")
        client_rpc_ret_str_list.append(space8+param.name+"=(char **)malloc(result_rpc->"+param.name+".twocharptr.twocharptr_len*sizeof(char *));")
        client_rpc_ret_str_list.append(space8+"if("+param.name+"==NULL) {")
        client_rpc_ret_str_list.append(space12+"perror(\"malloc failed\");")
        client_rpc_ret_str_list.append(space12+"exit(1);")
        client_rpc_ret_str_list.append(space8+"}")
        client_rpc_ret_str_list.append(space8+"for(int i=0;i<result_rpc->"+param.name+".twocharptr.twocharptr_len;i++) {")
        client_rpc_ret_str_list.append(space12+"if(result_rpc->"+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {")
        client_rpc_ret_str_list.append(space16+param.name+"[i]=NULL;")
        client_rpc_ret_str_list.append(space12+"} else {")
        client_rpc_ret_str_list.append(space16+param.name+"[i]=malloc(result_rpc->"+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));")
        client_rpc_ret_str_list.append(space12+"if("+param.name+"[i]==NULL) {")
        client_rpc_ret_str_list.append(space16+"perror(\"malloc failed\");")
        client_rpc_ret_str_list.append(space16+"exit(1);")
        client_rpc_ret_str_list.append(space12+"}")
        client_rpc_ret_str_list.append(space16+"memcpy("+param.name+"[i],result_rpc->"+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_val,result_rpc->"+param.name+".twocharptr.twocharptr_val[i].charptr.charptr_len);")
        client_rpc_ret_str_list.append(space8+"}")
        client_rpc_ret_str_list.append(space4+"}")

def dealwith_basicptr_client_server_param(typename,paramname,paramcount,wrapper_str_list,clnt_rpc_call_param_list,svc_param_list,unmashal_str_list,server_rpc_ret_str_list,client_rpc_ret_str_list):
    #客户端封装数据
    """
    int * count_rpc=NULL;
    if (count != NULL) {
        count_rpc.int_ptr_len = 1;  // 假设 count 数组的长度为 3
        count_rpc.int_ptr_val = malloc(sizeof(int) * count_rpc.int_ptr_len);  // 为 int_ptr_val 分配内存
        if (count_rpc.int_ptr_val == NULL) {
            perror("malloc failed");
            exit(1);
        }

        for (int i = 0; i < count_rpc.int_ptr_len; i++) {
            count_rpc.int_ptr_val[i] = count[i];  // 复制数据
        }
    }else{count_rpc.int_ptr_len=0;count_rpc.int_ptr_val=NULL;}

}
    """
    wrapper_str_list.append(typename+"_ptr "+paramname+"_rpc;")
    clnt_rpc_call_param_list.append(paramname+"_rpc")
    wrapper_str_list.append("if("+paramname+"!=NULL) {")
    wrapper_str_list.append(paramname+"_rpc."+typename+"_ptr_len=1;")
    wrapper_str_list.append(paramname+"_rpc."+typename+"_ptr_val=malloc(sizeof("+typename+")*"+paramname+"_rpc."+typename+"_ptr_len);")
    wrapper_str_list.append("if("+paramname+"_rpc."+typename+"_ptr_val==NULL) {")
    wrapper_str_list.append("perror(\"malloc failed\");")
    wrapper_str_list.append("exit(1);")
    wrapper_str_list.append("}")
    wrapper_str_list.append("for(int i0=0;i0<"+paramname+"_rpc."+typename+"_ptr_len;i0++) {")
    wrapper_str_list.append(paramname+"_rpc."+typename+"_ptr_val[i0]="+paramname+"[i0];")
    wrapper_str_list.append("\t}")
    wrapper_str_list.append("}else{"+paramname+"_rpc."+typename+"_ptr_len=0;"+paramname+"_rpc."+typename+"_ptr_val=NULL;}")
    

    
    #服务端解包数据
    """
    int * http_loop_sense_1_ret=NULL;
    if(arg1.int_ptr_val!=NULL) {
    http_loop_sense_1_ret=malloc(arg1.int_ptr_len*sizeof(int));
    if(http_loop_sense_1_ret==NULL) {
    perror("malloc failed");
    exit(1);
        }
    for(int i=0;i<arg1.int_ptr_len;i++) {
    http_loop_sense_1_ret[i]=arg1.int_ptr_val[i];
        }
    }
    """
    svc_param_list.append(typename+"_ptr arg"+str(paramcount))
    unmashal_str_list.append(typename+" *"+paramname+"=NULL;")
    unmashal_str_list.append("if(arg"+str(paramcount)+"."+typename+"_ptr_val!=NULL) {")
    unmashal_str_list.append(paramname+"=malloc(arg"+str(paramcount)+"."+typename+"_ptr_len*sizeof("+typename+"));")
    unmashal_str_list.append("if("+paramname+"==NULL) {")
    unmashal_str_list.append("perror(\"malloc failed\");")
    unmashal_str_list.append("exit(1);")
    unmashal_str_list.append("}")
    unmashal_str_list.append("for(int i0=0;i0<arg"+str(paramcount)+"."+typename+"_ptr_len;i0++) {")
    unmashal_str_list.append(paramname+"[i0]=arg"+str(paramcount)+"."+typename+"_ptr_val[i0];")
    unmashal_str_list.append("\t}")
    unmashal_str_list.append("}")

    """
    if (count != NULL) {
    result->paramname.int_ptr_len = 1;  // 假设 count 数组的长度为 3
    result->paramname.int_ptr_val = malloc(sizeof(int) * result->paramname.int_ptr_len);  // 为 int_ptr_val 分配内存
    if (result->paramname.int_ptr_val == NULL) {
        perror("malloc failed");
        exit(1);
    }

    for (int i = 0; i < result->paramname.int_ptr_len; i++) {
        result->paramname.int_ptr_val[i] = count[i];  // 复制数据
    }
    """
    #服务端封装返回值
    server_rpc_ret_str_list.append("if("+paramname+"==NULL) {result."+paramname+"."+typename+"_ptr_len=0;result."+paramname+"."+typename+"_ptr_val=NULL;}")
    server_rpc_ret_str_list.append("else {")
    server_rpc_ret_str_list.append("\tresult."+paramname+"."+typename+"_ptr_len=1;")
    server_rpc_ret_str_list.append("\tresult."+paramname+"."+typename+"_ptr_val=malloc(sizeof("+typename+")*result."+paramname+"."+typename+"_ptr_len);")
    server_rpc_ret_str_list.append("\tif(result."+paramname+"."+typename+"_ptr_val==NULL) {")
    server_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
    server_rpc_ret_str_list.append("\t\texit(1);")
    server_rpc_ret_str_list.append("\t}")
    server_rpc_ret_str_list.append("\tfor(int i0=0;i0<result."+paramname+"."+typename+"_ptr_len;i0++) {")
    server_rpc_ret_str_list.append("\t\tresult."+paramname+"."+typename+"_ptr_val[i0]="+paramname+"[i0];")
    server_rpc_ret_str_list.append("\t}")
    server_rpc_ret_str_list.append("}")


    #客户端解封装返回值
    client_rpc_ret_str_list.append("if(result_rpc->"+paramname+"."+typename+"_ptr_val==NULL) {")
    client_rpc_ret_str_list.append(paramname+"=NULL;")
    client_rpc_ret_str_list.append("} else {")
    client_rpc_ret_str_list.append(paramname+"=(int *)malloc(result_rpc->"+paramname+"."+typename+"_ptr_len*sizeof(int));")
    client_rpc_ret_str_list.append("if("+paramname+"==NULL) {")
    client_rpc_ret_str_list.append("perror(\"malloc failed\");")
    client_rpc_ret_str_list.append("exit(1);")
    client_rpc_ret_str_list.append("}")
    client_rpc_ret_str_list.append("for(int i0=0;i0<result_rpc->"+paramname+"."+typename+"_ptr_len;i0++) {")
    client_rpc_ret_str_list.append(paramname+"[i0]=result_rpc->"+paramname+"."+typename+"_ptr_val[i0];")
    client_rpc_ret_str_list.append("}")
    client_rpc_ret_str_list.append("}")


def dealwith_basicptr_client_server_retvalue(typename, server_rpc_ret_str_list, client_rpc_ret_str_list):
    #服务端封装返回值
    """
    if (sensefun_ret == NULL) {result->ret0.typename_ptr_len = 0;relust->ret0.typename_ptr_val = NULL;}
    else {
        result->ret0.typename_ptr_len = 1;
        for (int i = 0; i < result->ret0.typename_ptr_len; i++) {
        result->ret0.typename_ptr_val[i] = sensefun_ret[i];
        }
    }
    """
    server_rpc_ret_str_list.append("if (sensefun_ret == NULL) {result->ret0."+typename+"_ptr_len = 0;relust->ret0."+typename+"_ptr_val = NULL;}")
    server_rpc_ret_str_list.append("else {")
    server_rpc_ret_str_list.append("\tresult->ret0."+typename+"_ptr_len = 1;")
    server_rpc_ret_str_list.append("\tfor (int i = 0; i < result->ret0."+typename+"_ptr_len; i++) {")
    server_rpc_ret_str_list.append("\t\tresult->ret0."+typename+"_ptr_val[i] = sensefun_ret[i];")
    server_rpc_ret_str_list.append("\t}")
    server_rpc_ret_str_list.append("}")

    #客户端解封装返回值
    """
    if (result_rpc->ret0.typename_ptr_val == NULL) {sensefun_ret = NULL;}
    else {
        sensefun_ret = (typename *)malloc(result_rpc->ret0.typename_ptr_len * sizeof(typename));
        if (sensefun_ret == NULL) {
            perror("malloc failed");
            exit(1);
        }
        for (int i = 0; i < result_rpc->ret0.typename_ptr_len; i++) {
            sensefun_ret[i] = result_rpc->ret0.typename_ptr_val[i];
        }
    """
    client_rpc_ret_str_list.append("if (result_rpc->ret0."+typename+"_ptr_val == NULL) {sensefun_ret = NULL;}")
    client_rpc_ret_str_list.append("else {")
    client_rpc_ret_str_list.append("\tsensefun_ret = ("+typename+" *)malloc(result_rpc->ret0."+typename+"_ptr_len * sizeof("+typename+"));")
    client_rpc_ret_str_list.append("\tif (sensefun_ret == NULL) {")
    client_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
    client_rpc_ret_str_list.append("\t\texit(1);")
    client_rpc_ret_str_list.append("\t}")
    client_rpc_ret_str_list.append("\tfor (int i = 0; i < result_rpc->ret0."+typename+"_ptr_len; i++) {")
    client_rpc_ret_str_list.append("\t\tsensefun_ret[i] = result_rpc->ret0."+typename+"_ptr_val[i];")
    client_rpc_ret_str_list.append("\t}")
    client_rpc_ret_str_list.append("}")

def recur_dealwith_basicptr_server_retvalue(leftname,righname,opname,typename, server_rpc_ret_str_list):
    #服务端封装返回值
    server_rpc_ret_str_list.append("if ("+righname+" == NULL) {"+leftname+opname+typename+"_ptr_len = 0;"+leftname+opname+typename+"_ptr_val = NULL;}")
    server_rpc_ret_str_list.append("else {")
    server_rpc_ret_str_list.append("\t"+leftname+opname+typename+"_ptr_len = 1;")
    server_rpc_ret_str_list.append("\tfor (int i = 0; i < "+leftname+opname+typename+"_ptr_len; i++) {")
    server_rpc_ret_str_list.append("\t\t"+leftname+opname+typename+"_ptr_val[i] = "+righname+"[i];")
    server_rpc_ret_str_list.append("\t}")
    server_rpc_ret_str_list.append("}")

def recur_dealwith_basicptr_client_retvalue(leftname,righname,opname,typename, client_rpc_ret_str_list):
    #客户端解封装返回值
    client_rpc_ret_str_list.append("if ("+righname+"_rpc->"+typename+"_ptr_val == NULL) {"+leftname+" = NULL;}")
    client_rpc_ret_str_list.append("else {")
    client_rpc_ret_str_list.append("\t"+leftname+" = ("+typename+" *)malloc("+righname+"_rpc->"+typename+"_ptr_len * sizeof("+typename+"));")
    client_rpc_ret_str_list.append("\tif ("+leftname+" == NULL) {")
    client_rpc_ret_str_list.append("\t\tperror(\"malloc failed\");")
    client_rpc_ret_str_list.append("\t\texit(1);")
    client_rpc_ret_str_list.append("\t}")
    client_rpc_ret_str_list.append("\tfor (int i = 0; i < "+righname+"_rpc->"+typename+"_ptr_len; i++) {")
    client_rpc_ret_str_list.append("\t\t"+leftname+"[i] = "+righname+"_rpc->"+typename+"_ptr_val[i];")
    client_rpc_ret_str_list.append("\t}")
    client_rpc_ret_str_list.append("}")

def dealwith_stat_client_server_retvalue(server_rpc_ret_str_list, client_rpc_ret_str_list):
    #服务端封装返回值
    server_rpc_ret_str_list.append("result->ret0.st_dev=sensefun_ret.st_dev;")
    server_rpc_ret_str_list.append("result->ret0.st_ino=sensefun_ret.st_ino;")
    server_rpc_ret_str_list.append("result->ret0.st_mode=sensefun_ret.st_mode;")
    server_rpc_ret_str_list.append("result->ret0.st_nlink=sensefun_ret.st_nlink;")
    server_rpc_ret_str_list.append("result->ret0.st_uid=sensefun_ret.st_uid;")
    server_rpc_ret_str_list.append("result->ret0.st_gid=sensefun_ret.st_gid;")
    server_rpc_ret_str_list.append("result->ret0.st_rdev=sensefun_ret.st_rdev;")
    server_rpc_ret_str_list.append("result->ret0.st_size=sensefun_ret.st_size;")
    server_rpc_ret_str_list.append("result->ret0.st_blksize=sensefun_ret.st_blksize;")
    server_rpc_ret_str_list.append("result->ret0.st_blocks=sensefun_ret.st_blocks;")
    server_rpc_ret_str_list.append("result->ret0.st_atime=sensefun_ret.st_atime;")
    server_rpc_ret_str_list.append("result->ret0.st_mtime=sensefun_ret.st_mtime;")
    server_rpc_ret_str_list.append("result->ret0.st_ctime=sensefun_ret.st_ctime;")

    #客户端解封装返回值
    client_rpc_ret_str_list.append("sensefun_ret.st_dev=result_rpc->ret0.st_dev;")
    client_rpc_ret_str_list.append("sensefun_ret.st_ino=result_rpc->ret0.st_ino;")
    client_rpc_ret_str_list.append("sensefun_ret.st_mode=result_rpc->ret0.st_mode;")
    client_rpc_ret_str_list.append("sensefun_ret.st_nlink=result_rpc->ret0.st_nlink;")
    client_rpc_ret_str_list.append("sensefun_ret.st_uid=result_rpc->ret0.st_uid;")
    client_rpc_ret_str_list.append("sensefun_ret.st_gid=result_rpc->ret0.st_gid;")
    client_rpc_ret_str_list.append("sensefun_ret.st_rdev=result_rpc->ret0.st_rdev;")
    client_rpc_ret_str_list.append("sensefun_ret.st_size=result_rpc->ret0.st_size;")
    client_rpc_ret_str_list.append("sensefun_ret.st_blksize=result_rpc->ret0.st_blksize;")
    client_rpc_ret_str_list.append("sensefun_ret.st_blocks=result_rpc->ret0.st_blocks;")
    client_rpc_ret_str_list.append("sensefun_ret.st_atim=result_rpc->ret0.st_atim;")
    client_rpc_ret_str_list.append("sensefun_ret.st_mtim=result_rpc->ret0.st_mtim;")
    client_rpc_ret_str_list.append("sensefun_ret.st_ctim=result_rpc->ret0.st_ctim;")

def recur_dealwith_stat_server_retvalue(leftname,righname,opname,server_rpc_ret_str_list):
    #服务端封装返回值
    server_rpc_ret_str_list.append(leftname+opname+"st_dev="+righname+opname+"st_dev;")
    server_rpc_ret_str_list.append(leftname+opname+"st_ino="+righname+opname+"st_ino;")
    server_rpc_ret_str_list.append(leftname+opname+"st_mode="+righname+opname+"st_mode;")
    server_rpc_ret_str_list.append(leftname+opname+"st_nlink="+righname+opname+"st_nlink;")
    server_rpc_ret_str_list.append(leftname+opname+"st_uid="+righname+opname+"st_uid;")
    server_rpc_ret_str_list.append(leftname+opname+"st_gid="+righname+opname+"st_gid;")
    server_rpc_ret_str_list.append(leftname+opname+"st_rdev="+righname+opname+"st_rdev;")
    server_rpc_ret_str_list.append(leftname+opname+"st_size="+righname+opname+"st_size;")
    server_rpc_ret_str_list.append(leftname+opname+"st_blksize="+righname+opname+"st_blksize;")
    server_rpc_ret_str_list.append(leftname+opname+"st_blocks="+righname+opname+"st_blocks;")
    server_rpc_ret_str_list.append(leftname+opname+"st_atim="+righname+opname+"st_atime;")
    server_rpc_ret_str_list.append(leftname+opname+"st_mtim="+righname+opname+"st_mtime;")
    server_rpc_ret_str_list.append(leftname+opname+"st_ctim="+righname+opname+"st_ctime;")

def recur_dealwith_stat_client_retvalue(leftname,righname,opname,client_rpc_ret_str_list):
    #客户端解封装返回值
    client_rpc_ret_str_list.append(leftname+opname+"st_dev="+righname+opname+"st_dev;")
    client_rpc_ret_str_list.append(leftname+opname+"st_ino="+righname+opname+"st_ino;")
    client_rpc_ret_str_list.append(leftname+opname+"st_mode="+righname+opname+"st_mode;")
    client_rpc_ret_str_list.append(leftname+opname+"st_nlink="+righname+opname+"st_nlink;")
    client_rpc_ret_str_list.append(leftname+opname+"st_uid="+righname+opname+"st_uid;")
    client_rpc_ret_str_list.append(leftname+opname+"st_gid="+righname+opname+"st_gid;")
    client_rpc_ret_str_list.append(leftname+opname+"st_rdev="+righname+opname+"st_rdev;")
    client_rpc_ret_str_list.append(leftname+opname+"st_size="+righname+opname+"st_size;")
    client_rpc_ret_str_list.append(leftname+opname+"st_blksize="+righname+opname+"st_blksize;")
    client_rpc_ret_str_list.append(leftname+opname+"st_blocks="+righname+opname+"st_blocks;")
    client_rpc_ret_str_list.append(leftname+opname+"st_atim="+righname+opname+"st_atim;")
    client_rpc_ret_str_list.append(leftname+opname+"st_mtim="+righname+opname+"st_mtim;")
    client_rpc_ret_str_list.append(leftname+opname+"st_ctim="+righname+opname+"st_ctim;")
