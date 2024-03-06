import { CliArg } from "./argument";
export interface Option {
    /** 属性所属的目标对象 */
    target: any;
    /** 选项指向的属性名 */
    property: string;
    /** 短选项，格式固定为 /^-[a-zA-Z0-1]$/ */
    short?: string;
    /** 长选项 */
    long?: string;
    /** 选项简介 */
    brief?: string;
    /** 是否必须 */
    required?: boolean;
    /** 是否可重复，可重复的属性被定义为原类型的数组 */
    repeat?: boolean;
    /**
     * 选项参数
     * 若为字符串数组，则表示仅接受一个参数，且参数必须为数组中的某个字符串
     * 若为函数，则表示自定义参数检查
     *  函数返回 true 表示参数合法
     *  返回 false 表示参数不合法，打印默认错误信息
     *  返回 Error 表示参数不合法且需要输出错误信息
     */
    arg?: string[] | ((arg: string) => boolean | Error | Promise<boolean | Error>);
    /**
     * 参数自动补全
     * @param arg 可能存在的正在编辑的参数
     * @return 若返回 undefined 则表示不支持自动补全
     */
    complete?: ((arg?: string) => string[] | Promise<string[]>);
}
export declare function GetOptionDescTable(target: any, create?: boolean): undefined | {
    [key: string]: Option;
};
export declare function GetOptionDesc(target: any, propertyKey: string): Option;
export declare function unfoldShortOptions(args: string[]): void;
export declare function parseOptions(target: any, defines: {
    [key: string]: Option;
}, args: CliArg[]): Promise<boolean>;
export declare function verifyOptionDefinitions(options: {
    [key: string]: Option;
}, local_names: Set<string>): boolean;
