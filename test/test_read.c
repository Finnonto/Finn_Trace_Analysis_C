def find_est_lcg_16384_stageAverage(self, hash_type):
        stage_para = [
            104, 2154, 7507, 12230, 14702, 15742, 16144, 16295, 16351, 16372, 16380, 16383, 16384
        ]

        def est_lcg_16384_stage(container):
            # parameter
            table_size = 16384
            
            # function
            

            def hash_affine_40para(in_data, table_size):
                para_a = [
                    0x177510d1, 0xda1e0f42, 0x964fbf1e, 0x269df1e6, 0x916cc092,
                    0x7931bd51, 0x12a504fc, 0x76100f01, 0xc246978c, 0x87f2cc91, 
                    0x97910a77, 0x49a930b6, 0x3c48cc20, 0xbbbbff4e, 0xca2d6493, 
                    0xfabac315, 0xefbb19dd, 0x954361d, 0x5bff105e, 0x1ad1e815, 
                    0xa4d41053, 0xa5507e69, 0x9f571b50, 0x5f03492f, 0x35b3e590, 
                    0x426796b6, 0x1a462f78, 0x5f7404e2, 0xcb5e5215, 0x8f081cb7, 
                    0x95ab0a84, 0x7d2fd9ba, 0xd7d748db, 0x2c3137eb, 0x8c0c1e71, 
                    0x7eeab7d9, 0x4297c61f, 0xbaeec404, 0x55e56436, 0xe281d41a
                ]
                para_c = [
                    0x930874a7, 0x7df6e6b4, 0xc3e1046a, 0x289efd5b, 0xbe9694c1, 
                    0x2d85c18e, 0x3bfbe5bc, 0xb0e2d294, 0x5031b9cf, 0xe554adbb, 
                    0x7a6ea2de, 0x829a2955, 0x6b80de1d, 0x6cf04e75, 0x277f56b1, 
                    0x301c379c, 0xf65eeac1, 0xd7ab0949, 0xef0ded41, 0xf2cba001, 
                    0xe50f46da, 0xae65910f, 0xb2ec9527, 0x5241b84f, 0x6eb745dc, 
                    0x8b14934a, 0xe3087d58, 0xac7035bc, 0xac6c7471, 0xd0a517bc, 
                    0x71d65fe2, 0xbab80ff2, 0x224de591, 0xe433876b, 0x7485409b, 
                    0x7f067133, 0xa97e19bf, 0x9f3a5ff9, 0x5a545530, 0x6a115f65
                ]
                mod_m = 2**31-1

                hash_result = []
                for i in range(self.k_value):
                    result = (para_a[i]*in_data + para_c[i])%mod_m
                    key_a = result % table_size

                    result = (para_a[i+20]*in_data + para_c[i+20])%mod_m
                    key_b = result % table_size

                    hash_result.append( (key_a, key_b) )
                return hash_result

            def lcg(in_data, table_size):
                para_a = 22695477 # LCG gcc parameter
                para_c = 1 # LCG gcc parameter
                mod_m = 2**32

                lcg_result = []
                result = in_data
                for i in range(self.k_value):
                    result = (para_a * result + para_c) % mod_m
                    key_a = result % table_size
                    
                    result = (para_a * result + para_c) % mod_m
                    key_b = result % table_size

                    lcg_result.append( (key_a, key_b) )
                return lcg_result
            
            
            
            ## parameter
            k_register = [0,] * self.k_value
            total_item_cnt = 0
            entropy = 0
            
            ## read results and calculate
            for item, cnt in container.most_common():
                # total cnt
                total_item_cnt += cnt
                
                # hash
                if hash_type == 'lcg': hash_result = lcg(item, table_size)
                elif hash_type == '20para': hash_result = hash_affine_20para(item, table_size)
                elif hash_type == '40para': hash_result = hash_affine_40para(item, table_size)
                
                # query table
                query_result = []
                for key in hash_result:
                    ## table head
                    u1 = (key[0]+1) / (table_size+1)
                    u2 = 1 / (table_size+1)
                    
                    w1 = math.pi * (u1-0.5)
                    w2 = -math.log(u2)
                    
                    ran1 = math.tan(w1) * (math.pi/2 - w1)
                    ran2 = math.log( w2 * math.cos(w1) / (math.pi/2-w1) )
                    ran = ran1 + ran2
                    
                    ### store k value
                    table_head = round(ran)
                    
                    ## table end
                    table_end_key = key[1] + 1
                    if table_end_key   <= stage_para[0]:  table_end_correction = 0
                    elif table_end_key <= stage_para[1]:  table_end_correction = -1
                    elif table_end_key <= stage_para[2]:  table_end_correction = -2
                    elif table_end_key <= stage_para[3]:  table_end_correction = -3
                    elif table_end_key <= stage_para[4]:  table_end_correction = -4
                    elif table_end_key <= stage_para[5]:  table_end_correction = -5
                    elif table_end_key <= stage_para[6]:  table_end_correction = -6
                    elif table_end_key <= stage_para[7]:  table_end_correction = -7
                    elif table_end_key <= stage_para[8]:  table_end_correction = -8
                    elif table_end_key <= stage_para[9]:  table_end_correction = -9
                    elif table_end_key <= stage_para[10]: table_end_correction = -10
                    elif table_end_key <= stage_para[11]: table_end_correction = -11
                    elif table_end_key <= stage_para[12]: table_end_correction = -12
                    query_result.append( table_head + table_end_correction )
                
                # store k value
                for i in range(self.k_value):
                    k_register[i] += query_result[i] * cnt
            
            ## est entropy
            if total_item_cnt ==0 or total_item_cnt == 1: return None
            else: 
                for i in range(self.k_value):
                    k_register[i] /= total_item_cnt
                    entropy += math.exp(k_register[i])
                entropy /= self.k_value
                entropy = -math.log(entropy)
                return entropy
                
        return est_lcg_16384_stage